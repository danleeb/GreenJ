/**
 * @package    Javascript
 * @subpackage Base
 * @copyright  Copyright (c) 2012 Lorem Ipsum Mediengesellschaft m.b.H. (http://www.loremipsum.at)
 *
 * GNU General Public License
 * This file may be used under the terms of the GNU General Public License
 * version 3 as published by the Free Software Foundation and
 * appearing in the file LICENSE.GPL included in the packaging of this file.
 *
 */

/**
 * @name window
 * @namespace global namespace
 *
 * @name window.li
 * @namespace
 * @see li
 */

/**
 * @namespace global li namespace
 */
li = {};
if (typeof window.console === 'undefined') {
    /** @ignore */
    window.console = {};
}
if (typeof window.console.log === 'undefined') {
    /** @ignore */
    window.console.log = function(v) {};
}

(function(li) {

    /**
     * Simulates PHP's date function,
     * see <a href="http://jacwright.com/projects/javascript/date_format">jacwright.com/projects/javascript/date_format<a>
     */
    Date.prototype.format = function(format){var returnStr='';var replace=Date.replaceChars;for(var i=0;i<format.length;i++){var curChar=format.charAt(i);if(i-1>=0&&format.charAt(i-1)=="\\"){returnStr+=curChar;}else if(replace[curChar]){returnStr+=replace[curChar].call(this);}else if(curChar!="\\"){returnStr+=curChar;}}return returnStr;};Date.replaceChars={shortMonths:['Jan','Feb','Mar','Apr','May','Jun','Jul','Aug','Sep','Oct','Nov','Dec'],longMonths:['January','February','March','April','May','June','July','August','September','October','November','December'],shortDays:['Sun','Mon','Tue','Wed','Thu','Fri','Sat'],longDays:['Sunday','Monday','Tuesday','Wednesday','Thursday','Friday','Saturday'],d:function(){return(this.getDate()<10?'0':'')+this.getDate();},D:function(){return Date.replaceChars.shortDays[this.getDay()];},j:function(){return this.getDate();},l:function(){return Date.replaceChars.longDays[this.getDay()];},N:function(){return this.getDay()+1;},S:function(){return(this.getDate()%10==1&&this.getDate()!=11?'st':(this.getDate()%10==2&&this.getDate()!=12?'nd':(this.getDate()%10==3&&this.getDate()!=13?'rd':'th')));},w:function(){return this.getDay();},z:function(){var d=new Date(this.getFullYear(),0,1);return Math.ceil((this-d)/86400000);},W:function(){var d=new Date(this.getFullYear(),0,1);return Math.ceil((((this-d)/86400000)+d.getDay()+1)/7);},F:function(){return Date.replaceChars.longMonths[this.getMonth()];},m:function(){return(this.getMonth()<9?'0':'')+(this.getMonth()+1);},M:function(){return Date.replaceChars.shortMonths[this.getMonth()];},n:function(){return this.getMonth()+1;},t:function(){var d=new Date();return new Date(d.getFullYear(),d.getMonth(),0).getDate()},L:function(){var year=this.getFullYear();return(year%400==0||(year%100!=0&&year%4==0));},o:function(){var d=new Date(this.valueOf());d.setDate(d.getDate()-((this.getDay()+6)%7)+3);return d.getFullYear();},Y:function(){return this.getFullYear();},y:function(){return(''+this.getFullYear()).substr(2);},a:function(){return this.getHours()<12?'am':'pm';},A:function(){return this.getHours()<12?'AM':'PM';},B:function(){return Math.floor((((this.getUTCHours()+1)%24)+this.getUTCMinutes()/60+this.getUTCSeconds()/3600)*1000/24);},g:function(){return this.getHours()%12||12;},G:function(){return this.getHours();},h:function(){return((this.getHours()%12||12)<10?'0':'')+(this.getHours()%12||12);},H:function(){return(this.getHours()<10?'0':'')+this.getHours();},i:function(){return(this.getMinutes()<10?'0':'')+this.getMinutes();},s:function(){return(this.getSeconds()<10?'0':'')+this.getSeconds();},u:function(){var m=this.getMilliseconds();return(m<10?'00':(m<100?'0':''))+m;},e:function(){return"Not Yet Supported";},I:function(){return"Not Yet Supported";},O:function(){return(-this.getTimezoneOffset()<0?'-':'+')+(Math.abs(this.getTimezoneOffset()/60)<10?'0':'')+(Math.abs(this.getTimezoneOffset()/60))+'00';},P:function(){return(-this.getTimezoneOffset()<0?'-':'+')+(Math.abs(this.getTimezoneOffset()/60)<10?'0':'')+(Math.abs(this.getTimezoneOffset()/60))+':00';},T:function(){var m=this.getMonth();this.setMonth(0);var result=this.toTimeString().replace(/^.+ \(?([^\)]+)\)?$/,'$1');this.setMonth(m);return result;},Z:function(){return-this.getTimezoneOffset()*60;},c:function(){return this.format("Y-m-d\\TH:i:sP");},r:function(){return this.toString();},U:function(){return this.getTime()/1000;}};

    /**
     * Same as {@link Date.now}
     * @return this
     */
    Date.prototype.now = function() {
        this.setTime(Date.now().getTime());
        return this;
    };
    /**
     * Returns a Date object with the current datetime.
     * @return {Date}
     */
    Date.now = function() {
        return new Date();
    };

    /** jQuery version */
    li.jquery = jQuery().jquery;

    /**
     * li extend base functionality
     */
    jQuery.extend(li, /** @lends li */ {
        /**
         * This method adds all properties from supertype.prototype to child.prototype,
         * but does not override existing properties in child.
         * <pre>
         *  li.inherit(AdvancedObject, BaseObject);	// BaseObject properties don't override AdvancedObject properties
         * </pre>
         * @param {Object} child           Object, that inherits supertype
         * @param {Object} supertype       Object, that is inherited by child
         * @return {Object} child
         */
        inherit: function(child, supertype) {
            var property;
            for (property in supertype.prototype) {
                if (!(property in child.prototype)) {
                    child.prototype[property] = supertype.prototype[property];
                }
            }
            return child;
        },
        /**
         * This method adds properties from supertype.prototype to child.prototype
         * and overrides existing propierties in child.
         * <pre>
         *  li.extend(AdvancedObject, ObjectExtension);     // ObjectExtension properties replace AdvancedObject properties
         * </pre>
         * @param {Object} child           Object, that is extended supertype
         * @param {Object} supertype       Object, that extends child
         * @returns {Object} child
         */
        extend: function(child, supertype) {
            var property;
            for (property in supertype.prototype) {
                child.prototype[property] = supertype.prototype[property];
            }
            return child;
        }
    });

    /**
     * Empty class.
     * @class Object to identify event listeners.
     */
    li.EventListener = function() {};

    /**
     * Base class object that provides helper methods, event listener funtionality,
     *  timeout/interval methods and more.
     * @class Provides base functionality and is inherited by other li objects
     */
    li.BaseObject = function() {};
    li.BaseObject.prototype = {
        /**
         * Add a listener callback function to an event
         * @param {string} eventName            Name of the event, the listener is attached to
         * @param {Function} listenerCallback   Callback function, that is called when event is triggered
         * @return {li.EventListener} Object to identify listener (use deleteListener(obj))
         */
        addListener: function(eventName, listenerCallback) {
            this.__listeners = this.defaults(this.__listeners, {});
            eventName = eventName.split(' ');
            var i,l = eventName.length, ret = [];
            for (i=0;i < l;i++) {
                var listenerObj = new li.EventListener();
                this.__listeners[eventName[i]] = this.defaults(this.__listeners[eventName[i]], []);
                this.__listeners[eventName[i]].push({
                    listener: listenerObj,
                    callback: listenerCallback
                });
                ret.push(listenerObj);
            }
            return (ret.length === 1) ? ret[0] : ret;
        },
        /**
         * Trigger event
         * @param {string} eventName    Name of the event, that should be triggered
         * @param {mixed} data          Parameter (e.g. Object) that is given to the event listeners callback functions
         * @return {Array} List of return values of all callback methods.
         */
        trigger: function(eventName, data) {
            if (!this.__listeners || typeof this.__listeners[eventName] === 'undefined') {
                return [];
            }
            data = this.defaults(data, {});
            var index, eventListeners = this.__listeners[eventName], retArr = [];
            for (index in eventListeners) {
                if (eventListeners.hasOwnProperty(index)) {
                    retArr.push(eventListeners[index].callback.call(this, data));
                }
            }
            return retArr;
        },
        /**
         * Delete all listeners of all events.
         */
        deleteAllListeners: function() {
            this.__listeners = {};
        },
        /**
         * Delete all listeners of a specific event.
         * @param {string} eventName     Name of the event, whose listeners should be deleted
         */
        deleteListeners: function(eventName) {
            if (!this.__listeners || typeof this.__listeners[eventName] === 'undefined') {
                return;
            }
            delete this.__listeners[eventName];
        },
        /**
         * Delete a specific listener
         * @param {li.EventListener} listenerObj     The listener object that should be deleted.
         * @return {boolean} true, if listener was successfully deleted.
         */
        deleteListener: function(listenerObj) {
            if (this.__listeners) {
                var index, i;
                for (index in this.__listeners) {
                    if (this.__listeners.hasOwnProperty(index)) {
                        for (i in this.__listeners[index]) {
                            if (this.__listeners[index].hasOwnProperty(i)
                                && this.__listeners[index][i].listener === listenerObj)
                            {
                                this.__listeners[index].splice(i, 1);
                                return true;
                            }
                        }
                    }
                }
            }
            return false;
        },

        /**
         * Helper to set default parameter.
         * use like: <code>function(x) { x = this.defaults(x, 0); }</code>
         * TODO: more advanced version using argument list! or at least object to set multiple parameters.
         * @param parameter
         * @param defaultValue
         * @param {string} type [optional] if parameter should be a certain type: e.g. 'string', ...
         * @return defaultValue, if parameter is undefined, otherwise parameter.
         */
        defaults: function(parameter, defaultValue, type) {
            if (typeof type === 'undefined') {
                return (typeof parameter !== 'undefined') ? parameter : defaultValue;
            }
            return (typeof parameter === type) ? parameter : defaultValue;
        },
        /**
         * Returns true, if the variable is not undefined
         */
        isset: function(v) {
            return (typeof v !== 'undefined') ? true : false;
        },

        /**
         * Override setInterval method
         * @param {string} id           name as identifier for the interval
         * @param {Function} callback   intervals callback function
         * @param {integer} millisec    interval time in milliseconds
         * @param {boolean} overwrite   [optional] should an existing interval (id) be overwritten?
         * @return interval-id generated by setInterval() function or null
         */
        setInterval: function(id, callback, millisec, overwrite) {
            overwrite = this.defaults(overwrite, true);
            this.__intervals = this.defaults(this.__intervals, {});
            if (false === overwrite && typeof this.__intervals[id] !== 'undefined' && this.__intervals[id] !== null) {
                return this.__intervals[id];
            }
            if (false === li.errorHandler.blockIntervals()) {
                return (this.__intervals[id] = setInterval(callback, millisec));
            }
            return null;
        },
        /**
         * Override clearInterval method
         * @param {string} id     name as identifier of the interval, that should be cleared
         */
        clearInterval: function(id) {
            if (this.__intervals && typeof this.__intervals[id] !== 'undefined' && this.__intervals[id] !== null) {
                clearInterval(this.__intervals[id]);
                this.__intervals[id] = null;
            }
        },
        /**
         * Override setTimeout method
         * @param {string} id           name as identifier for the timeout
         * @param {Function} callback   timeouts callback function
         * @param {integer} millisec    timeout time in milliseconds
         * @param {boolean} overwrite   [optional] should an existing timeout (id) be overwritten?
         * @return timeout-id generated by setTimeout() function or null
         */
        setTimeout: function(id, callback, millisec, overwrite) {
            overwrite = this.defaults(overwrite, true);
            this.__timeouts = this.defaults(this.__timeouts, {});
            if (false === overwrite && typeof this.__timeouts[id] !== 'undefined' && this.__timeouts[id] !== null) {
                return this.__timeouts[id];
            }
            if (false === li.errorHandler.blockTimeouts()) {
                return (this.__timeouts[id] = setTimeout(callback, millisec));
            }
            return null;
        },
        /**
         * Override clearTimeout method
         * @param {string} id     name as identifier of the timeout, that should be cleared
         */
        clearTimeout: function(id) {
            if (this.__timeouts && typeof this.__timeouts[id] !== 'undefined' && this.__timeouts[id] !== null) {
                clearTimeout(this.__timeouts[id]);
                this.__timeouts[id] = null;
            }
        }

    };

    jQuery.extend(li, li.BaseObject.prototype);

    /**
     * li extend errorHandler
     */
    jQuery.extend(li, /** @lends li */ {
        /** Object with error types as name->code pairs */
        errorType: {
            OK:                         0,
            DEFAULT:                    1,
            ABSTRACT:                   2,

            AJAX_FAILED:                10,
            AJAX_JSON_FAILED:           11
        },

        /**
         * The Error Handler object.
         * Events:
         * <pre>
         *  'onError'       triggered, when an error occured (even if options.debug = false!)
         *                  { errorCode: code, errorMessage: message[code], errorData: output }
         *  'onLog'         triggered, when a log message has been sent to the errorHandler
         *                  { message: logMessage }
         * </pre>
         * @namespace Namespace in li for error handling.
         * @augments li.BaseObject
         */
        errorHandler: {
            /** errorHandler options. */
            options: {
                debug:				false,
                bubbleException:	false,
                alert:				false,
                throwException:     false,
                console:			false,
                render:             false,
                renderCallback:     null,
                output:				null,
                exception:          null,
                blockIntervals:     false,
                blockTimeouts:      false,

                log:                true,
                logTo:              null
            },
            /** Array with error messages. */
            message:        [],
            /** Array with error code names. */
            errorCodeName:  [],

            /**
             * Init errorHandler and set options
             */
            init: function(options) {
                jQuery.extend(this.options, options);
                var name;
                for (name in li.errorType) {
                    if (li.errorType.hasOwnProperty(name)) {
                        this.errorCodeName[li.errorType[name]] = name;
                    }
                }			
            },
            /**
             * Update errorHandler: should be called when new messages are added
             */
            update: function() {
                var name;
                for (name in li.errorType) {
                    if (li.errorType.hasOwnProperty(name) && typeof this.errorCodeName[li.errorType[name]] === 'undefined') {
                        this.errorCodeName[li.errorType[name]] = name;
                    }
                }
            },
            _isCode: function(code) {
                if (typeof code === 'number') {
                    var name;
                    for (name in li.errorType) {
                        if (li.errorType.hasOwnProperty(name) && li.errorType[name] === code) {
                            return true;
                        }
                    }
                }
                return false;
            },
            /** log message */
            log: function(v, logTo) {
                if (!this.options.log) { return; }
                if (this.isset(console) && this.isset(console.log)) {
                    console.log(v);
                }
                this.trigger('onLog', { message: v });
                this._renderLog(logTo ? logTo : this.options.logTo, v);
            },
            /**
             * Trigger error with code and options
             * @param {integer} code        error code, should be a {@link li.errorType}
             * @param {Object} options      extend {@link li.errorHandler.options}
             * @throws {string|integer} if throwException is true, throws an error message or error code.
             */
            add: function(code, options) {
                code = this.defaults(code, li.errorType.DEFAULT);
                options = this.defaults(options, {});
                var cloneOptions = jQuery.extend({}, this.options);
                jQuery.extend(cloneOptions, options);
                var output = (cloneOptions.output === null) ? '' : cloneOptions.output;
                if (cloneOptions.exception !== null && this._isCode(cloneOptions.exception)) {
                    code = cloneOptions.exception;
                }
                if (typeof this.message[code] === 'string') {
                    this.trigger('onError', { errorCode: code, errorMessage: this.message[code], errorData: output });
                    if (cloneOptions.debug) {
                        if (cloneOptions.alert) { alert(this.message[code] + ": " + output); }
                        if (cloneOptions.console && this.isset(console) && this.isset(console.log)) {
                            console.log(this.message[code]);
                            console.log(output);
                            if (cloneOptions.exception) { console.log(cloneOptions.exception); }
                        }
                        if (cloneOptions.log) {
                            this._renderLog(cloneOptions.logTo, this.message[code]);
                            this._renderLog(cloneOptions.logTo, output);
                        }
                        if (cloneOptions.render) { this._render(code, this.message[code], output, cloneOptions); }
                        if (cloneOptions.throwException) { throw this.message[code] + output; }
                    }
                } else {
                    var msg = 'Error ' + ((typeof this.errorCodeName[code] !== 'undefined') ? this.errorCodeName[code] : '') + '(' + code + ')';
                    this.trigger('onError', { errorCode: code, errorMessage: msg, errorData: output });
                    if (cloneOptions.debug) {
                        if (cloneOptions.alert) { alert(msg + ": " + output); }
                        if (cloneOptions.console && this.isset(console) && this.isset(console.log)) {
                            console.log(msg);
                            console.log(output);
                            if (cloneOptions.exception) { console.log(cloneOptions.exception); }
                        }
                        if (cloneOptions.log) {
                            this._renderLog(cloneOptions.logTo, msg);
                            this._renderLog(cloneOptions.logTo, output);
                        }
                        if (cloneOptions.render) { this._render(code, msg, output, cloneOptions); }
                        if (cloneOptions.throwException) { throw code; }
                    }
                }
                return code;
            },
            /**
             * Trigger Alert-Error, that outputs error via alert().
             * uses {@link li.errorHandler.add}
             */
            doAlert: function(code, output) {
                output = this.defaults(output, null);
                this.add(code, { alert: true, output: output });
            },
            /**
             * Create Throw-Error that throws an exception.
             * uses {@link li.errorHandler.add}
             */
            doThrow: function(code, output) {
                output = this.defaults(output, null);
                this.add(code, { throwException: true, output: output });
            },
            /**
             * Create Console-Error, that outputs error in console.
             * uses {@link li.errorHandler.add}
             */
            doConsole: function(code, output) {
                output = this.defaults(output, null);
                this.add(code, { console: true, output: output });
            },
            /**
             * Create Error and render it.
             * uses {@link li.errorHandler.add}
             */
            doRender: function(code, output) {
                output = this.defaults(output, null);
                this.add(code, { render: true, output: output });
            },
            /**
             * Throws exception ex, if options.bubbleException is set to true
             * @param ex     Exception
             */
            bubbleException: function(ex) {
                if (this.options.bubbleException) {
                    throw ex;
                }
            },
            /**
             * @return {boolean} true, if intervals should be blocked (used by li.BaseObject.setInterval())
             */
            blockIntervals: function() {
                return (this.options.blockIntervals) ? true : false;
            },
            /**
             * @return {boolean} true, if timeouts should be blocked (used by li.BaseObject.setTimeout())
             */
            blockTimeouts: function() {
                return (this.options.blockTimeouts) ? true : false;
            },
            /** Calls the renderCallback method or uses a default rendering */
            _render: function(code, msg, output, options) {
                if (options.renderCallback !== null) {
                    return options.renderCallback.call(this, code, msg, output, options);
                }
                var message = "", stack = null;
                message += this.defaults(msg, '', 'string');
                message += ': ';
                message += output;
                message += ' (' + this.errorCodeName[code] + ', ' + code + ')';

                if (typeof options.output !== 'undefined' && options.output !== null && typeof options.output.XMLHttpRequest !== 'undefined') {
                    message += '<br /><br /><button id="__showResponseButton__">Show response</button>';
                    try {
                        var json = jQuery.parseJSON(options.output.XMLHttpRequest.responseText);
                        stack = json.stack;
                        message += '<br /><button id="__showStackButton__">Show stack</button>';
                    } catch (ex) {
                    }
                }

                var element = jQuery('<div></div>').html(message);
                element.dialog({
                        autoOpen:    true,
                        title:       'Error',
                        modal:       true,
                        dialogClass: 'error',
                        close: function(event, ui) {
                            element.dialog('destroy');
                            element.remove();
                            jQuery('#__showResponseLayer__').remove();
                            jQuery('#__showStackLayer__').remove();
                        }
                    });

                if (typeof options.output !== 'undefined' && options.output !== null && typeof options.output.XMLHttpRequest !== 'undefined') {
                    var requestObj = options.output.XMLHttpRequest;
                    jQuery('#__showResponseButton__').click(function() {
                        jQuery('<div id="__showResponseLayer__" style="position:absolute; left:0px; top:0px; z-index:11111; width:100%; height:700px; overflow:scroll; background:#ddd;"></div>').html(requestObj.responseText).appendTo('body').click(function() { jQuery(this).remove(); });
                    });
                    jQuery('#__showStackButton__').click(function() {
                        if (stack) {
                            jQuery('<div id="__showStackLayer__" style="position:absolute; left:0px; top:0px; z-index:11111; width:100%; height:700px; overflow:scroll; background:#ddd;"></div>').html(stack).appendTo('body').click(function() { jQuery(this).remove(); });
                        }
                    });
                }
            },
            /** Used by log() method to render log entry to an element. */
            _renderLog: function(selector, v) {
                if (selector === null || selector === '') {
                    return;
                }
                jQuery(selector).append('<p>'+v+'</p>')
                    .attr({ scrollTop: jQuery(selector).attr("scrollHeight") });
            }
        }
    });
    jQuery.extend(li.errorHandler, li.BaseObject.prototype);

}(window.li));