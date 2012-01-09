var settings = {
    host:   '',
    name:   '',
    secret: ''
};

jQuery(document).ready(function () {
    jQuery.blockUI({
        css: {
            border: 'none',
            padding: '15px',
            backgroundColor: '#000',
            '-webkit-border-radius': '10px',
            '-moz-border-radius': '10px',
            opacity: 0.5,
            color: '#fff'
        },
        message: '<h1><img style="vertical-align:middle;" src="images/ajax-loader.gif" /> Verbindung zum Server wird hergestellt ...</h1>'
    });
    var current_call = null;
    var flip = jQuery("#quickflip-wrapper").quickFlip();

    jQuery(".incoming-calls").button();
    var accept_button = jQuery(".accept").button({});
    jQuery(".number, .hangup").button();

    /**
     * Scrolling class
     * This allows scrolling by mouse-position
     * which is used in the call-list and call-history-list
     */
    function makeScrollable(wrapper, scrollable) {
        // Get jQuery elements
        wrapper = jQuery(wrapper);
        scrollable = jQuery(scrollable);

        // Hide images until they are not loaded
        scrollable.hide();

        function enable() {
            // height of area at the top at bottom, that don't respond to mousemove
            var inactiveMargin = 100;
            // Cache for performance
            var wrapperHeight = wrapper.height();
            // Using outer height to include padding too
            var scrollableHeight = scrollable.outerHeight() + 2 * inactiveMargin;
            // Do not cache wrapperOffset, because it can change when user resizes window
            // We could use onresize event, but it&#39;s just not worth doing that
            // var wrapperOffset = wrapper.offset();

            //When user move mouse over menu
            wrapper.mousemove(function (e) {
                var wrapperOffset = wrapper.offset();
                // Scroll menu
                var top = (e.pageY -  wrapperOffset.top) * (scrollableHeight - wrapperHeight) / wrapperHeight  - inactiveMargin;

                if (top < 0) {
                    top = 0;
                }

                wrapper.scrollTop(top);
            });
        }
        var loading = jQuery("<div class='loading'></div>").appendTo(wrapper);

        // Set function that will check if all images are loaded
        var interval = setInterval(function () {
            var images = scrollable.find("img");
            var completed = 0;

            // Counts number of images that are succesfully loaded
            images.each(function () {
                if (this.complete) {
                    completed++;
                }
            });

            if (completed === images.length) {
                clearInterval(interval);
                // Timeout added to fix problem with Chrome
                setTimeout(function () {

                    loading.hide();
                    // Remove scrollbars
                    wrapper.css({overflow: "hidden"});

                    scrollable.slideDown("slow", function () {
                        enable();
                    });
                }, 1000);
            }
        }, 100);
    }

    /**
     * Function resizes the inputfield to the length of content
     * so that the protocol an server-information stays next to the field
     */
    function resizeInputField(input_field, input_val) {
        var input_str = input_field.val();
        if (input_val) {
            input_str = input_val;
        }

        if (!input_str) {
            jQuery(".inputadditional").hide();
        } else {
            jQuery(".inputadditional").show();
        }
        var text = jQuery('<span>')
            .html(input_str)
            .appendTo(jQuery("#quickflip-wrapper").parent());
        var width = text.innerWidth();
        text.remove();

        input_field.width(width + 10);
    }
    resizeInputField(jQuery("#numstring"));

    /**
     * This function handles call-accept-functionality
     * It tells the program to accept an incoming call or
     * to make a new call.
     */
    function callAccept() {
        var incoming_id = jQuery('input[name=test]:checked').val();
        var c = null;
        if (current_call) {
            jQuery('<div title="Fehler">Anruf läuft gerade</div>')
                .dialog({dialogClass: 'ui-state-error'});
            return;
        }

        var image = "";
        var number = "";

        if (incoming_id) {
            c = window.phone.getCallById(Number(incoming_id));
            c.accept();
            image = "images/incoming.png";
            number = c.getNumber();
        } else {
            number = jQuery("#numstring").val();
            if (!number) {
                jQuery('<div title="Fehler">Keine Nummer gewählt</div>')
                    .dialog({dialogClass: 'ui-state-error'});
                return false;
            }
            c = window.phone.makeCall({protocol: "sip:", number: number});
            image = "images/outgoing.png";
        }
        var button_el = jQuery('<button class="button-history" value="' + number
                               + '"><span class="history-number">' + number + '</span>'
                               + '<span class="time"> (' + c.callTime.format("H:i:s") + ')</span>'
                               + '<span class="history-image"><image src="'
                               + image + '" /></span></button>');
        var button = jQuery(button_el).button();
        button.click(function () {
            jQuery("#numstring").val(jQuery(this).val());
            flip.quickFlipper();
            resizeInputField(jQuery("#numstring"), jQuery(this).val());
        });
        jQuery('#call-history').append(button);


        jQuery('.deactivateable').button('disable');
        current_call = c;

        c.addListener('onClose', function () {
            if (c === current_call) {
                jQuery('.deactivateable').button('enable');
                current_call = null;
            }
        });
    }

    /**
     * action to the number-buttons
     * they shoult print their value into the inputfield
     */
    jQuery(".number").live("click", function () {
        var current = jQuery('input[name=test]:checked');
        current.val('');
        current.button("refresh");
        var number = jQuery("#numstring").val();
        number += jQuery(this).text();
        jQuery("#numstring").val(number);
        resizeInputField(jQuery("#numstring"));
        return false;
    });
    /**
     * binds pressing key to input-field
     * so you need not focusing it by mouse
     */
    jQuery(document).bind("keydown", function () {
        jQuery("#numstring").focus();
    });
    /**
     * resizes the input-field after every key-release
     */
    jQuery("#numstring").live("keyup", function () {
        resizeInputField(jQuery(this));
    });
    /**
     * accept a call when pressing enter
     */
    jQuery("#numstring").live("keypress", function (event) {
        if (event.which === 13 && !current_call) {
            callAccept();
            event.preventDefault();
        }
    });

    /**
     * access to the phone
     */
    window.phone = new li.Phone({
        qthandler: window.qt_handler,
        mode:      li.Phone.MODE_IO
    });
    window.qt_handler.registerJsCallbackHandler("window.phone" + window.phone.getHandlerName());

    window.phone.addListener('onIncomingCall', function (data) {
        if (typeof data.incomingCall === 'undefined' || false === data.incomingCall) {
            return li.errorHandler.add(li.errorType.ERROR);
        }
        var c = data.incomingCall;
        var html = '';
        var num = jQuery('<div/>').text(c.getFullNumber()).html();
        html += '<hr />[' + c.id + '] incoming call<br/>';
        html += '[' + c.id + '] number: ' + num + ' | ' + c.getNumber();
        jQuery('#incoming').append(html);
        c.addListener('onAccept', function () {
            window.phone.clearInterval('callInterval' + c.id);
            jQuery('#incoming').append('<br />[' + c.id + '] accepted');
            jQuery('.deactivateable').button('disable');
            current_call = c;
        });
        var button_el = jQuery('<div class="button-list" id="list-element-' + c.id +
                               '"><input type="radio" class="incoming-calls deactivateable" id="radio' + c.id +
                               '" name="test" value="' + c.id + '"/><label id="label' + c.id + '" for="radio' + c.id +
                               '">' + c.getNumber() + ' <span class="time">(' +  c.callTime.format("H:i:s") + ' - </span><span id="counter' + c.id + '" class="time">0)</span></label></div>');
        window.phone.setInterval('callInterval' + c.id, function () {
            var now = Date.now();
            var t = Math.round((now.getTime() - c.callTime.getTime()) * 0.001);
            jQuery("#counter" + c.id).text(t + ')');
        }, 1000);
        jQuery('#call-list').append(button_el);
        jQuery('#radio' + c.id).button();

        c.addListener('onClose', function () {
            jQuery('#incoming').append('<br />[' + c.id + '] closed');
            jQuery('#list-element-' + c.id).remove();
            accept_button.button("option", "label", "Anrufen");
            if (c === current_call) {
                jQuery('.deactivateable').button('enable');
                current_call = null;
            } else {
                var button_el = jQuery('<button class="button-history" value="'
                                       + c.getNumber() + '"><span class="history-number">'
                                       + c.getNumber() + '</span><span class="time"> ('
                                       + c.callTime.format("H:i:s") + ')</span>'
                                       + '<span class="history-image">'
                                       + '<image src="images/cross.png" /></span></button>');
                var button = jQuery(button_el).button();
                button.click(function () {
                    jQuery("#numstring").val(jQuery(this).val());
                    flip.quickFlipper();
                    resizeInputField(jQuery("#numstring"));
                });
                jQuery('#call-history').append(button);
            }
        });
        if (current_call) {
            jQuery('#radio' + c.id).button('disable');
        }
        makeScrollable(".wrapper", "#call-list");

    });

    /**
     * listener to print log messages
     */
    window.phone.addListener('onLogMessage', function (obj) {
        var txt = obj.time + ': (' + obj.status + ') <' + obj.domain + '> [' + obj.code + '] ' + obj.message;
        jQuery('#log').append('<div>' + txt + '</div>');
    });

    /**
     * listener to print error messages
     */
    li.errorHandler.addListener('onError', function (data) {
        jQuery('#log').append('<div>' + data.errorMessage + ': ' +
                              data.errorData + '</div>');
    });

    window.phone.register(settings);

    jQuery(".hostname").text(window.phone.account.host);

    jQuery('.accept').live("click", function () {
        callAccept();
        return false;
    });
    jQuery('.hangup').live("click", function () {
        jQuery("#numstring").val("");
        resizeInputField(jQuery("#numstring"));
        if (current_call) {
            current_call.hangUp();
        } else {
            var incoming_id = jQuery('input[name=test]:checked').val();
            var c = null;
            if (incoming_id) {
                c = window.phone.getCallById(Number(incoming_id));
                c.hangUp();
            }
        }
        return false;
    });

    /**
     * listener which releases the phone-access when it gets
     * the confirmation of a successful account-login
     */
    window.phone.addListener("onAccountState", function (data) {
        if (data.state >= li.Phone.SIP_SC_BAD_REQUEST) {
            jQuery('<div title="Fehler">Registrierung fehlgeschlagen</div>').dialog({dialogClass: 'ui-state-error'});
        } else if (data.state === li.Phone.SIP_SC_OK) {
            jQuery("#container").show();
            jQuery.unblockUI();
        }
    });

    jQuery(".button-list").live('click', function () {
        accept_button.button("option", "label", "Annehmen");
    });


    jQuery(".revert").live("click", function () {
        makeScrollable(".history-wrapper", "#call-history");
        flip.quickFlipper();
        jQuery(".ui-button").button("enable");

    });

    jQuery("#display").click(function () {
        jQuery("#numstring").focus();
    });
});
