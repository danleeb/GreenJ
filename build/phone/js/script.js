jQuery(document).ready(function($) {
    var settings = {
        host:           null,   // 'localhost',
        sipaccount:     'user',
        sipsecret:      'pw',
        qthandler:      window.qt_handler,
        //forceOutgoing:  null,
        mode:           li.Phone.MODE_IO
    };
    
    /**
     * Todo:
     * - Improve persisting of calls (accepted, missed, times, data?)
     * - settings panel
     *      - change host/username/password (save in localStorage)
     *      - change phone mode (MODE_IO, MODE_IN, MODE_OUT)
     *      - set forceOutgoingNumber
     *      - clear log history
     *      - set maxCallLogs value
     *      - debug settings (errorHandler, phone log level)
     *      - show/hide log
     * - log:
     *      - show list of log files: getLogFileList
     *      - show content of log file: getLogFileContent
     *      - delete log file: deleteLogFile
     * - connection to server / server-less calls
     *      - show registration/account status
     * - compact/detailed list switchers (contacts, calls)
     *      - compact list: auto-switch if col is too small
     * - show contact in call log and "add contact" if number matches
     * - statistics (in settings?)
     *      - avg. call time / call
     *      - total call time
     *      - total calls (incoming/outgoing)
     *  - contact search
     *  [- print contact list]
     */
    
    //-------------------------------------------------------------------------
    // Application
    var app = {
        maxCallLogs: 30,
        text: {
            newContact: 'New contact'
        },
        
        phone: null,
        db: window.localStorage,
        contacts: [],
        calls: [],
        
        exec: function() {
            if (this.db.contacts) {
                var contacts = JSON.parse(this.db.contacts), i;
                for (i = contacts.length - 1; i >= 0; i--) {
                    this.addContact(contacts[i], true);
                }
            }
            if (this.db.calls) {
                var calls = JSON.parse(this.db.calls), i;
                for (i = 0; i < calls.length; i++) {
                    this.addCallLog(calls[i], true);
                }
            }
        },
        exit: function() {
        },
        registerPhone: function() {
            this.phone.register({
                host:   settings.host,
                name:   settings.sipaccount,
                secret: settings.sipsecret
            });
        },
        addContact: function(contact, skipPersist) {
            var self = this;
            this.contacts.splice(0, 0, contact);
            var $contact = $('<div></div>').prependTo('#contacts .content');
            var $name = $('<input class="name" value="' + contact.name + '" title="Edit name" />').appendTo($contact)
                .trigger('blur')
                .change(function() {
                    var val = $.trim($(this).val()), inserted = false;
                    if (val === '') {
                        $(this).val(contact.name);
                    } else {
                        contact.name = val;
                    }
                    $contact.appendTo('#contacts .content');
                    for (var i = 0; i < self.contacts.length; i++) {
                        if (self.contacts[i] === contact) {
                            self.contacts.splice(i, 1);
                        }
                    }
                    // Sorted by name
                    for (var i = 0; i < self.contacts.length; i++) {
                        if (self.contacts[i].name.toLowerCase() > contact.name.toLowerCase()) {
                            $('#contacts .content > div').eq(i).before($contact);
                            self.contacts.splice(i, 0, contact);
                            inserted = true;
                            break;
                        }
                    }
                    if (!inserted) {
                        self.contacts.push(contact);
                    }
                    self.persistContacts();
                })
                .blur(function() {
                    if ($(this).val() === self.text.newContact) {
                        $(this).trigger('change');
                    }
                });
            var $number = $('<input class="number" value="' + contact.number + '" title="Edit number" />').appendTo($contact)
                .trigger('blur')
                .change(function() {
                    var val = $.trim($(this).val());
                    if (val === '') {
                        $(this).val(contact.number);
                    } else {
                        contact.number = val;
                    }
                    self.persistContacts();
                });
            var $select = $('<button class="select">Select</button>').appendTo($contact)
                .click(function() {
                    $('#number').val(contact.number).trigger('change');
                });
            var $delete = $('<button class="delete" title="Double-click to delete contact">X</button>').appendTo($contact)
                .dblclick(function() {
                    for (var i = 0; i < self.contacts.length; i++) {
                        if (self.contacts[i] === contact) {
                            self.contacts.splice(i, 1);
                        }
                    }
                    $contact.remove();
                    $('#contacts').tinyscrollbar_update('relative');
                    self.persistContacts();
                });
            if (!skipPersist) {
                this.persistContacts();
            }
        },
        persistContacts: function() {
            this.db.contacts = JSON.stringify(this.contacts);
        },
        addCall: function(call) {
            var $call = this.addCallLog({ 
                    number: call.getNumber(), 
                    fullnumber: call.getFullNumber(), 
                    outgoing: call.isOutgoing(), 
                    date: call.getData().callTime
                }).removeClass('closed');
            var $accept = null;
            if (call.isIncoming()) {
                $accept = $('<button class="accept">Accept</button>').appendTo($call)
                    .click(function() {
                        $('#number').val(call.getNumber()).trigger('change');
                        call.accept();
                    });
            }
            var $close = $('<button class="close">HangUp</button>').appendTo($call)
                .click(function() {
                    call.hangUp();
                });
            var $ringing = $('<div class="time"></div>').appendTo($call);
            call.setInterval('ringTime', function() {
                    $ringing.text(call.getRingTime());
                }, 500, false, true);
            call.addListener('onAccept', function() {
                this.clearInterval('ringTime');
                call.setInterval('callTime', function() {
                    $ringing.text(call.getCallTime());
                }, 500, false, true);
                $call.addClass('accepted');
                if ($accept) {
                    $accept.remove();
                }
            });
            call.addListener('onClose', function() {
                this.clearInterval('ringTime');
                this.clearInterval('callTime');
                $call.addClass('closed');
                $close.remove();
                if ($accept) {
                    $accept.remove();
                }
            });
            $('#calls').tinyscrollbar_update();
        },
        addCallLog: function(call, skipPersist) {
            var $call = $('<div></div>').prependTo('#calls .content');
            $call.addClass(call.outgoing ? 'outgoing' : 'incoming').addClass('closed');
            var date = new Date(call.date), now = new Date(), format = '';
            if (date.getYear() !== now.getYear()) {
                format += 'Y-m-j ';
            } else if (date.getMonth() !== now.getMonth() || date.getDate() !== now.getDate()) {
                format += 'M-j ';
            }
            $call.append('<dt title="' + date.format('Y-m-j G:i:s') + '">' + date.format(format + 'G:i') + ' - ' + (call.outgoing ? 'Outgoing' : 'Incoming') + '</dt>');
            $call.append('<dd title="' + call.fullnumber + '">' + call.number + '</dd>');
            var $select = $('<button class="select">Select</button>').appendTo($call)
                .click(function() {
                    $('#number').val(call.number).trigger('change');
                });
            this.calls.push(call);
            if (!skipPersist) {
                this.persistCalls();
            }
            return $call;
        },
        persistCalls: function() {
            if (this.calls.length > this.maxCallLogs) {
                this.calls.splice(0, this.calls.length - this.maxCallLogs);
            }
            this.db.calls = JSON.stringify(this.calls);
        },
        selectTab: function(tab) {
            if ($('body').hasClass('col1')) {
                $('#' + tab).show().siblings().hide();
                $('#nav' + tab).addClass('active').siblings().removeClass('active');
            } else if ($('body').hasClass('col2')) {
                if (tab !== 'calls') {
                    $('#' + tab).show().siblings('#phone, #contacts').hide();
                    $('#nav' + tab).addClass('active').siblings('#navphone, #navcontacts').removeClass('active');
                }
            }
        }
    };
    $.extend(app, li.BaseObject.prototype);
    
    // Configurate error handler
    li.errorHandler.init({
    	debug:				true,
        log:				true,
        bubbleException:	false,      // true, if you want to catch Exceptions here
        alert:				false,
        throwException: 	false,
        console:			true,
        render:				true,
        blockIntervals:		false,      // true, to disable all intervals
        blockTimeouts:		false       // true, to disable all timeouts
    });
    
    //-------------------------------------------------------------------------
    // Phone
    try {
        li.phone = app.phone = new li.Phone({
            qthandler: 				settings.qthandler,
            forceOutgoingNumber:	settings.forceOutgoing,
            mode:					settings.mode
        });
        settings.qthandler.registerJsCallbackHandler("window.li.phone" + app.phone.getHandlerName());
        
        app.phone.addListener('onRegister', function() {
            $('#sound .ui-slider').slider('value', this.getSoundLevel());
            $('#micro .ui-slider').slider('value', this.getMicroLevel());
            if (this.isMutePhoneSound()) {
                $('#sound .button').addClass('mute');
            }
            if (this.isMutePhoneMicro()) {
                $('#micro .button').addClass('mute');
            }
        });
        app.phone.addListener('onAccountState', function(data) {
            if (data.state < li.Phone.SIP_SC_BAD_REQUEST) {
                app.exec();
            } else {
                app.exit();
                this.unregister();
                li.errorHandler.add(li.errorType.PHONE_ERROR, { output: data.state });
            }
        });
        app.phone.addListener('onIncomingCall', function(data) {
            if (typeof data.incomingCall === 'undefined' || false === data.incomingCall) {
                return li.errorHandler.add(li.errorType.ERROR);
            }
            app.selectTab('calls');
            app.addCall(data.incomingCall);
        });
        // Listeners to print log and error messages
        app.phone.addListener('onLogMessage', function(obj) {
            $('#log').append('<div>' + obj.time + ': (' + obj.status + ') <' + obj.domain + '> [' 
                                     + obj.code + '] ' + obj.message + '</div>');
        });
        li.errorHandler.addListener('onError', function(data) {
            $('#log').append('<div>' + data.errorMessage + ': ' + data.errorData + '</div>');
        });
        li.errorHandler.addListener('onLog', function(data) {
            $('#log').append('<div>' + data.message + '</div>');
        });
        
        // Check, if there are some errors from the last execution of GreenJ
        var data = app.phone.getErrorLog();
        if (data && data.length > 0) {
            li.errorHandler.add(li.errorType.PHONE_DEBUG, { output: data });
            app.phone.clearErrorLog();
        }
	} catch (e) {
		li.errorHandler.add(li.errorType.DEFAULT, { exception: e } );
	}
    
    //-------------------------------------------------------------------------
    // Elements
    $('#log').hide();
    $('#numblock button').click(function() {
        var button = $(this).text(),
            val = $('#number').val();
        if (button === '<') {
            val = val.slice(0, -1);
        } else {
            val += button;
        }
        $('#number').val(val).trigger('change').focus();
    });
    $('#number').change(function() {
        app.selectTab('phone');
        if ($.trim($(this).val()) === '') {
            $('#clearNumber').hide();
            $('#addNumber').css('visibility', 'hidden');
        } else {
            $('#clearNumber').show();
            $('#addNumber').css('visibility', 'visible');
        }
    });
    $('#clearNumber').click(function() {
        $('#number').val('').trigger('change').focus();
    });
    $('#addNumber').click(function() {
        app.addContact({ name: app.text.newContact, number: $.trim($('#number').val()) });
        app.selectTab('contacts');
        $('#contacts .name').first().focus();
        $('#contacts').tinyscrollbar_update();
    });
    
    $('#buttonCall').click(function() {
        var number = $.trim($('#number').val());
        if (number.length) {
            try {
                var call = app.phone.makeCall({ number: number });
                if (call) {
                    app.addCall(call);
                    app.selectTab('calls');
                }
            } catch (e) {
                li.errorHandler.add(li.errorType.PHONE_ERROR, { exception: e, output: e } );
            }
        }
    });
    $('#buttonHangUpAll').click(function() {
        try {
            app.phone.hangUpAll();
        } catch (e) {
            li.errorHandler.add(li.errorType.PHONE_ERROR, { exception: e, output: e } );
        }
    });
    
    $('#navcontacts').click(function() { app.selectTab('contacts'); });
    $('#navphone').click(function() { app.selectTab('phone'); });
    $('#navcalls').click(function() { app.selectTab('calls'); });
    
    $('#contacts').tinyscrollbar();
    $('#calls').tinyscrollbar();
    $('<div class="slider"><div></div></div>').appendTo('#sound, #micro').hide().find('div')
        .slider({
			orientation: "vertical",
			range: "min",
			min: 0,
			max: 255,
			value: 60
		});
    $('#sound .ui-slider').slider('option', 'slide', function(event, ui) {
            app.phone.setSoundLevel(ui.value);
        });
    $('#micro .ui-slider').slider('option', 'slide', function(event, ui) {
            app.phone.setMicroLevel(ui.value);
        });
    $('#sound, #micro')
        .hover(function() {
            $(this).find('.slider').show();
        }, function() {
            $(this).find('.slider').hide();
        });
    $('#sound .button').click(function() {
            $(this).toggleClass('mute');
            app.phone.mutePhoneSound($(this).hasClass('mute'));
        });
    $('#micro .button').click(function() {
            $(this).toggleClass('mute');
            app.phone.mutePhoneMicro($(this).hasClass('mute'));
        });
    
    $('#contacts .name, #contacts .number').live('focus', function() {
            $(this).css('width', '100%');
        }).live('blur', function() {
            var $ruler = jQuery("<div>" + $(this).val() + "</div>")
                .addClass($(this).attr("class")).css('visibility', 'hidden').css('width', 'auto').css('display', 'inline-block')
                .appendTo('#contacts');
            $(this).css('width', $ruler[0].offsetWidth);
            $ruler.remove();
        });
    
    if (!settings.host) {
        app.exec();
    }
    
    var cols = 0;
    $(window).resize(function() {
        var width = $(window).width();
        if (width < 500) {
            if (cols !== 1) {
                cols = 1;
                $('body').removeClass('col2 col3').addClass('col1');
                $('#phone').show().siblings().hide();
                $('#navphone').addClass('active').siblings().removeClass('active');
            }
        } else if (width < 770) {
            if (cols !== 2) {
                cols = 2;
                $('body').removeClass('col1 col3').addClass('col2');
                $('#contacts').hide().siblings().show();
                $('#navcontacts').removeClass('active').siblings().addClass('active');
            }
        } else if (cols !== 3) {
            cols = 3;
            $('body').removeClass('col1 col2').addClass('col3');
            $('#contacts, #phone, #calls').show();
            $('#navcontacts, #navphone, #navcalls').addClass('active');
        }
        $('#contacts').tinyscrollbar_update('relative');
        $('#calls').tinyscrollbar_update('relative');
    }).trigger('resize');
    
    $(document).keydown(function(event) {
        if (!$(event.target).is('input')) {
            $('#number').focus();
        }
    });

    app.registerPhone();
});