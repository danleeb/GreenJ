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
     * - Header panel (color, selected)
     * - Sort contacts by name (at insert and rename)
     * - maxCallLogLength: cut array on persisting (not on load)
     * - Log show/hide "button"
     * - Improve persisting of calls (data?)
     * - call number!
     * - settings panel
     * - connection to server / server-less calls
     * - EditName/EditNumber dynamic input size
     * - if no input has focus, send keypress to number input
     */
    
    //-------------------------------------------------------------------------
    // Application
    var app = {
        phone: null,
        maxCallLogLength: 30,
        db: window.localStorage,
        contacts: [],
        calls: [],
        
        exec: function() {
            this.db.calls = "";
            if (this.db.contacts) {
                var contacts = JSON.parse(this.db.contacts), i;
                for (i = 0; i < contacts.length; i++) {
                    this.addContact(contacts[i], true);
                }
            }
            if (this.db.calls) {
                var calls = JSON.parse(this.db.calls), i;
                var start = calls.length - this.maxCallLogLength;
                for (i = start > 0 ? start : 0; i < calls.length; i++) {
                    this.addCallLog(calls[i], true);
                }
            }
        },
        exit: function() {
        },
        registerPhone: function() {
            // Register phone
            this.phone.register({
                host:   settings.host,
                name:   settings.sipaccount,
                secret: settings.sipsecret
            });
        },
        addContact: function(contact, skipPersist) {
            var self = this;
            var pos = this.contacts.push(contact) - 1;
            var $contact = $('<div></div>').prependTo('#contacts .content');
            var $name = $('<input class="name" value="' + contact.name + '" title="Edit name" />').appendTo($contact)
                .change(function() {
                    var val = $(this).val();
                    if (val === '') {
                        $(this).val(contact.name);
                    } else {
                        contact.name = val;
                    }
                    self.persistContacts();
                });
            var $number = $('<input class="number" value="' + contact.number + '" title="Edit number" />').appendTo($contact)
                .change(function() {
                    var val = $(this).val();
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
                    self.contacts.splice(pos, 1);
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
            var $call = $('<div></div>').prependTo('#calls .content');
            $call.addClass(call.isOutgoing() ? 'outgoing' : 'incoming');
            $call.append('<dt>' + (call.isOutgoing() ? 'Outgoing' : 'Incoming') + '</dt>');
            $call.append('<dd title="' + call.getFullNumber() + '">' + call.getNumber() + '</dd>');
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
            var $select = $('<button class="select">Select</button>').appendTo($call)
                .click(function() {
                    $('#number').val(call.getNumber()).trigger('change');
                })
                .hide();
            call.addListener('onAccept', function() {
                $call.addClass('accepted');
                if ($accept) {
                    $accept.remove();
                }
            });
            call.addListener('onClose', function() {
                $call.addClass('closed');
                $select.show();
                $close.remove();
                if ($accept) {
                    $accept.remove();
                }
            });
            $('#calls').tinyscrollbar_update();
            this.calls.push({ number: call.getNumber(), fullnumber: call.getFullNumber(), outgoing: call.isOutgoing() });
            this.persistCalls();
        },
        addCallLog: function(call, skipPersist) {
            var $call = $('<div></div>').prependTo('#calls .content');
            $call.addClass(call.outgoing ? 'outgoing' : 'incoming').addClass('closed');
            $call.append('<dt>' + (call.outgoing ? 'Outgoing' : 'Incoming') + '</dt>');
            $call.append('<dd title="' + call.fullnumber + '">' + call.number + '</dd>');
            var $select = $('<button class="select">Select</button>').appendTo($call)
                .click(function() {
                    $('#number').val(call.number).trigger('change');
                });
            this.calls.push(call);
            if (!skipPersist) {
                this.persistCalls();
            }
        },
        persistCalls: function() {
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
        
        if (!settings.host) {
            app.exec();
        }
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
            var call = data.incomingCall;
            app.selectTab('calls');
            app.addCall(call);
        });
        // Listener to print log messages
        app.phone.addListener('onLogMessage', function(obj) {
            $('#log').append('<div>' + obj.time + ': (' + obj.status + ') <' + obj.domain + '> [' 
                                     + obj.code + '] ' + obj.message + '</div>');
        });
        // Listener to print error messages
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
            value = $('#number').val();
        if (button === '<') {
            value = value.slice(0, -1);
        } else {
            value += button;
        }
        $('#number').val(value).trigger('change').focus();
    });
    $('#number').change(function() {
        app.selectTab('phone');
        if ($(this).val() === '') {
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
        app.addContact({ name: 'New contact', number: $('#number').val() });
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
    
    $('#buttonMuteSound').change(function() {});
    $('#buttonMuteMicro').change(function() {});
    
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
            app.phone.muteMicroSound($(this).hasClass('mute'));
        });
    
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
    
    // Register phone
    app.registerPhone();
        
});