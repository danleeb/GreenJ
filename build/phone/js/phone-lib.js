/**
 * @package    Javascript
 * @subpackage Phone
 * @copyright  Copyright (c) 2012 Lorem Ipsum Mediengesellschaft m.b.H. (http://www.loremipsum.at)
 *
 * GNU General Public License
 * This file may be used under the terms of the GNU General Public License
 * version 3 as published by the Free Software Foundation and
 * appearing in the file LICENSE.GPL included in the packaging of this file.
 *
 */

(function(li) {

/**
 * Manages calls ({@link li.Phone.Call}) and uses {@link li.Phone.Handler} as an interface to the phone application.
 *  Triggers 'onOptionsChanged'
 * <pre>
 *  Events:                  Parameter (data)
 *      onIncomingCall      { incomingCall: li.Phone.Call object }
 *      onCallAccept        { id: call id }
 *      onCallClose         { id: call id }
 *      onError             { message: string, error message, [exception: e] }      triggered, when an error in a callback occured
 *      onLogMessage        { time: string              datetime string
 *                            status: uint              log level
 *                            domain: string
 *                            code: int
 *                            message: string
 *                          }
 *      onRegister          {}                                  after register() > registerToServer (if no exceptions are thrown)
 *      onUpdate            {}                                  after update()
 *      onRegisterUpdate    {}                                  after register() and first update()
 *      onUnregister        {}                                  after unregister() > unregisterFromServer
 *      onAccountState      { state: state }                    account state has changed
 *      onOptionsChanged    {}                                  after setOptions() (use this.options to read options)
 *      onMakeCall          { outgoingCall: li.Phone.Call }     after makeCall()
 *      onSoundLevelChanged integer level
 *      onMicroLevelChanged integer level
 *      onPrintUrl
 * </pre>
 * @class Phone class that handles and controls phone functionality.
 * @augments li.BaseObject
 * @param {Object} options  see {@link li.Phone#setOptions}
 * @property {Object} options               Phone options
 * @property {li.Phone.Handler} handler     Interface to the phone application
 * @property {Array} calls                  List of {@link li.Phone.Call}
 * @property {Object} account               Account information
 */
li.Phone = function(options) {
    this.handler = new li.Phone.Handler(this);
    this.calls = [];

    this.options = {
        qthandler:           null,
        forceOutgoingNumber: false,
        mutePhoneSound:      false,             // phone sound status (if true, effects all calls)
        mutePhoneMicro:      false,             // phone micro status (if true, effects all calls)
        soundLevel:          255,
        microLevel:          255,
        mode:                li.Phone.MODE_IO
    };
    this.setOptions(options);

    this.account = {
        host:           '',
        port:           5060,
        name:           '',
        secret:         '',
        address:        null,
        status:         0,
        onlineStatus:   null
    };

    var self = this;
    this.addListener('onIncomingCall', function(data) {
            self.update();
        });
    this.addListener('onError', function(data) {
            li.errorHandler.add(li.errorType.PHONE_ERROR, { output: data.message, exception: data.exception });
        });
    this.addListener('onCallAccept', function(data) {
            li.errorHandler.log("phone.onCallAccept("+data.id+"): phone.hasCallById="+this.hasCallById(data.id));
            if (!this.hasCallById(data.id)) {
                return;
            }
            this.getCallById(data.id).trigger('onAccept', data);
        });
    this.addListener('onCallClose', function(data) {
            li.errorHandler.log("phone.onCallClose("+data.id+"): phone.hasCallById="+this.hasCallById(data.id));
            if (!this.hasCallById(data.id)) {
                return;
            }
            this.getCallById(data.id).trigger('onClose', data);
        });
    this.addListener('onRegister', function() {
            var obj = self.getQtHandler().getSignalInformation();
            self.options.soundLevel = obj.sound;
            self.options.microLevel = obj.micro;
            self.options.mutePhoneSound = (obj.sound > 0 ? false : true);
            self.options.mutePhoneMicro = (obj.micro > 0 ? false : true);
        });
    this.addListener('onAccountState', function(data) {
            li.errorHandler.log("phone.onAccountState(): state="+data.state);
            self.account.status = data.state;
        });
};
jQuery.extend(li.Phone, {
    MODE_IO:                0,          // incoming and outgoing calls
    MODE_IN:                1,          // only incoming calls
    MODE_OUT:               2,          // only outgoing calls

    LOG_STATUS_DEBUG:       0,
    LOG_STATUS_MESSAGE:     1,
    LOG_STATUS_WARNING:     2,
    LOG_STATUS_ERROR:       3,
    LOG_STATUS_FATAL_ERROR: 4,
    
    SIP_SC_TRYING: 100, SIP_SC_RINGING: 180, SIP_SC_CALL_BEING_FORWARDED: 181, SIP_SC_QUEUED: 182, SIP_SC_PROGRESS: 183, 
    SIP_SC_OK: 200, SIP_SC_ACCEPTED: 202, 
    SIP_SC_MULTIPLE_CHOICES: 300, SIP_SC_MOVED_PERMANENTLY: 301, SIP_SC_MOVED_TEMPORARILY: 302, SIP_SC_USE_PROXY: 305, SIP_SC_ALTERNATIVE_SERVICE: 380, 
    SIP_SC_BAD_REQUEST: 400, SIP_SC_UNAUTHORIZED: 401, SIP_SC_PAYMENT_REQUIRED: 402, SIP_SC_FORBIDDEN: 403, SIP_SC_NOT_FOUND: 404, 
        SIP_SC_METHOD_NOT_ALLOWED: 405, SIP_SC_NOT_ACCEPTABLE: 406, SIP_SC_PROXY_AUTHENTICATION_REQUIRED: 407, SIP_SC_REQUEST_TIMEOUT: 408, SIP_SC_GONE: 410, 
        SIP_SC_REQUEST_ENTITY_TOO_LARGE: 413, SIP_SC_REQUEST_URI_TOO_LONG: 414, SIP_SC_UNSUPPORTED_MEDIA_TYPE: 415, SIP_SC_UNSUPPORTED_URI_SCHEME: 416, 
        SIP_SC_BAD_EXTENSION: 420, SIP_SC_EXTENSION_REQUIRED: 421, SIP_SC_SESSION_TIMER_TOO_SMALL: 422, SIP_SC_INTERVAL_TOO_BRIEF: 423, 
        SIP_SC_TEMPORARILY_UNAVAILABLE: 480, SIP_SC_CALL_TSX_DOES_NOT_EXIST: 481, SIP_SC_LOOP_DETECTED: 482, SIP_SC_TOO_MANY_HOPS: 483, 
        SIP_SC_ADDRESS_INCOMPLETE: 484, SIP_AC_AMBIGUOUS: 485, SIP_SC_BUSY_HERE: 486, SIP_SC_REQUEST_TERMINATED: 487, SIP_SC_NOT_ACCEPTABLE_HERE: 488, 
        SIP_SC_BAD_EVENT: 489, SIP_SC_REQUEST_UPDATED: 490, SIP_SC_REQUEST_PENDING: 491, SIP_SC_UNDECIPHERABLE: 493, 
    SIP_SC_INTERNAL_SERVER_ERROR: 500, SIP_SC_NOT_IMPLEMENTED: 501, SIP_SC_BAD_GATEWAY: 502, SIP_SC_SERVICE_UNAVAILABLE: 503, 
        SIP_SC_SERVER_TIMEOUT: 504, SIP_SC_VERSION_NOT_SUPPORTED: 505, SIP_SC_MESSAGE_TOO_LARGE: 513, SIP_SC_PRECONDITION_FAILURE: 580, 
    SIP_SC_BUSY_EVERYWHERE: 600, SIP_SC_DECLINE: 603, SIP_SC_DOES_NOT_EXIST_ANYWHERE: 604, SIP_SC_NOT_ACCEPTABLE_ANYWHERE: 606
});
li.Phone.prototype = {
    calls:          null,
    options:        null,
    account:        null,
    handler:        null,
    _registered:    false,

    /**
     * Set phone options
     *  Triggers 'onOptionsChanged'
     * @param {Object} options <pre> {
     *      qthandler:           qt handler object
     *      forceOutgoingNumber: false: don't force number, otherwise number as string
     *      mutePhoneSound:      boolean: phone sound status (if true, effects all calls)
     *      mutePhoneMicro:      boolean: phone micro status (if true, effects all calls)
     *      soundLevel:          int: sound level (0...255)
     *      microLevel:          int: microphone level (0...255)
     *      mode:                li.Phone.MODE_IO (default) or
     *                                   .MODE_IN (only incoming calls) or
     *                                   .MODE_OUT (only outgoing calls)
     *  } </pre>
     * @return this
     */
    setOptions: function(options) {
        options = this.defaults(options, {});
        jQuery.extend(this.options, options);
        this.trigger('onOptionsChanged');
        return this;
    },

    /**
     * Sends log object/message to the phone application.
     *  Triggers 'onLogMessage' with the given object.
     * @param {Object|string} obj   message or full log object: <pre> {
     *      time: string            datetime string; defaults to 'dd.mm.YYYY HH:MM:SS' of the current datetime
     *      status: uint            log level; defaults to li.Phone.LOG_STATUS_MESSAGE
     *      domain: string          defaults to 'phone-lib'
     *      code: int               defaults to 0
     *      message: string         log message
     *  } </pre>
     * @return this
     */
    sendLogMessage: function(obj) {
        var defObj = {
            time:       Date.now().format('d.m.Y H:i:s'),
            status:     li.Phone.LOG_STATUS_MESSAGE,
            domain:     'phone-lib',
            code:       0,
            message:    ''
        };
        if (typeof obj === 'string') {
            defObj.message = obj;
        } else {
            jQuery.extend(defObj, obj);
        }
        this.trigger('onLogMessage', defObj);
        this.getQtHandler().sendLogMessage(defObj);
        return this;
    },

    /**
     * Register account to server.
     *  Triggers 'onRegister' and after {@link li.Phone#update} 'onRegisterUpdate',
     *  even if account has already been registered.
     * @param {Object} account { host: string, name: string, secret: string }
     * @throws {li.errorType}.PHONE_REGISTRATION_ERROR or .PHONE_REGISTRATION_FAILED
     * @return {boolean} true, if successful (account has been registered)
     */
    register: function(account) {
        if (this.isRegistered()) {
            li.errorHandler.log("phone.register(): already registered!");
        } else {
            account = this.defaults(account, {});
            jQuery.extend(this.account, account);
            var ret = (this.getQtHandler().registerToServer(this.account.host,
                                                            this.account.name,
                                                            this.account.secret) ? true : false);
            li.errorHandler.log("phone.register('"+this.account.host+"', '"+this.account.name+"'): registerToServer returned " + ret);
            if (ret === false) {
                throw li.errorType.PHONE_REGISTRATION_ERROR;
            }
            if (!this.isRegistered()) {
                throw li.errorType.PHONE_REGISTRATION_FAILED;
            }
        }
        this.trigger('onRegister');
        this.update();
        this.trigger('onRegisterUpdate');
        return true;
    },
    /**
     * Unregister from server
     * Triggers 'onUnregister'.
     * @return {boolean} true, if successful, respectively account is not registered
     */
    unregister: function() {
        li.errorHandler.log("phone.unregister()");
        if (!this.isRegistered()) {
            return true;
        }
        this.getQtHandler().unregisterFromServer();
        this.trigger('onUnregister');
        return this.isRegistered() ? false : true;
    },
    /**
     * Is account registered? Checks account status on the phone application.
     * @return {boolean} true, if account is registered
     */
    isRegistered: function() {
        this._registered = (this.getQtHandler().checkAccountStatus() ? true : false);
        return this._registered;
    },
    /**
     * Get current mode.
     * @return li.Phone.MODE_IO, .MODE_OUT or .MODE_IN
     */
    getMode: function() {
        return this.options.mode;
    },
    /**
     * Updates account information and active calls.
     *  Triggers 'onUpdate'.
     * @return this
     */
    update: function() {
        li.errorHandler.log("phone.update()");
        var info = this.getQtHandler().getAccountInformation();
        this.account.address      = info.address;
        this.account.status       = info.status;
        this.account.onlineStatus = info.online_status;

        var i, calllist = this.getQtHandler().getActiveCallList();
        for (i in calllist) {
            if (calllist.hasOwnProperty(i)) {
                var opt = {
                    id:     calllist[i].id,
                    phone:  this
                };
                if (this.hasCallById(opt.id)) {
                    this.getCallById(opt.id).update(calllist[i]);
                } else {
                    var call = new li.Phone.Call(opt, calllist[i]);
                    try {
                        var str = this.getQtHandler().getCallUserData(calllist[i].id);
                        if (str !== "") {
                            var data = jQuery.parseJSON(str);
                            call.setUserData(data, false, false);
                        }
                    } catch (ex) {
                        li.errorHandler.log("phone.update(): could not parse user data.");
                    }
                }
            }
        }
        this.trigger('onUpdate');
        return this;
    },
    /**
     * Get the name of the handler object property.
     *  Used to register handler: Example:
     *  <code> window.qt_handler.registerJsCallbackHandler('window.phone' + phone.getHandlerName()); </code>
     * @return {String} name of the callback handler
     */
    getHandlerName: function() {
        return ".handler";
    },
    /**
     * Get callback handler.
     * @return {Object} Callback handler object
     */
    getHandler: function() {
        return this.handler;
    },
    /**
     * Get Qt handler.
     * @return {Object} Qt handler object
     */
    getQtHandler: function() {
        return this.options.qthandler;
    },
    /**
     * Adds a call to the phone object.
     * @param {li.Phone.Call} callObj
     * @return {Object|boolean} call or false, if a call with this id exists.
     */
    addCall: function(callObj) {
        if (this.hasCallById(callObj.id)) {
            return false;
        }
        callObj.phone = this;
        this.calls[callObj.id] = callObj;
        return this.calls[callObj.id];
    },
    /**
     * Get a list of all calls.
     * @return {Array}  List of (li.Phone.Call)
     */
    getCalls: function() {
        return this.calls;
    },
    /**
     * Checks, if there are calls [optional: that match the given conditions].
     * @param {Object} conditions <pre> {
     *      ringing:    true/false/null,
     *      accepted:   true/false/null,
     *      closed:     true/false/null,
     *      incoming:   true/false/null,
     *      outgoing:   true/false/null
     *  } </pre>
     * @return {boolean} true, if call(s) exist, otherwise false
     */
    hasCalls: function(conditions) {
        var i, l = this.calls.length;
        if (l < 1) {
            return false;
        }
        if (!conditions) {
            for (i = 0; i < l; i++) {
                if (this.isset(this.calls[i])) {
                    return true;
                }
            }
        } else {
            for (i = 0; i < l; i++) {
                if (this.isset(this.calls[i])) {
                    var call = this.calls[i], name, skip = false;
                    for (name in conditions) {
                        if (conditions.hasOwnProperty(name) && conditions[name] !== null) {
                            var func = 'is' + name.substr(0,1).toUpperCase() + name.substr(1);
                            if (conditions[name] !== call[func]()) { skip = true; break; }
                        }
                    }
                    if (!skip) {
                        return true;
                    }
                }
            }
        }
        return false;
    },
    /**
     * Checks, if a call with the id exists.
     * @param {integer} id
     * @return {boolean} true, if a call with the id exists
     */
    hasCallById: function(id) {
        return (typeof this.calls[id] !== 'undefined') ? true : false;
    },
    /**
     * Get a call by id
     * @param {integer} id
     * @return {li.Phone.Call} object with the id
     */
    getCallById: function(id) {
        return this.calls[id];
    },
    /**
     * Calls the callback method for each call; 'this' will be pointing to the current call in the callback method.
     * @param {Function} callback
     * @return this
     */
    eachCall: function(callback) {
        var i, l = this.calls.length;
        for (i = 0; i < l; i++) {
            if (this.isset(this.calls[i])) {
                callback.call(this.calls[i]);
            }
        }
        return this;
    },
    /**
     * Remove call from the calllist (does NOT close the call in the phone application!)
     * @param {li.Phone.Call|integer} callObj   object, that should be removed, or id of the call
     * @return {boolean} true, if the call has been removed
     */
    removeCall: function(callObj) {
        if (typeof callObj.id !== 'undefined') {
            callObj = callObj.id;
        }
        if (typeof this.calls[callObj] === 'undefined') {
            return false;
        }
        delete this.calls[callObj];
        return true;
    },
    /**
     * Removes all calls from the call array (does NOT close the calls in the phone application!)
     */
    removeAll: function() {
        this.calls = [];
    },
    /**
     * Initiates a call to a number.
     *  Triggers 'onMakeCall' with { outgoingCall: {@link li.Phone.Call} }.
     * @param {Object} options <pre> {
     *      number: string       number to call
     *      protocol: string     defaults to 'sip:'
     *      userdata: mixed      some user data
     *  } </pre>
     * @return {li.Phone.Call|boolean} Call that has been created or false, if no call has been created.
     * @throws {li.errorType}.PHONE_MAKECALL_NUMBERREQUIRED, .PHONE_MAKECALL_FAILED
     */
    makeCall: function(options) {
        //TODO: this should be handled by the pjsip wrapper!
        if (this.getMode() === li.Phone.MODE_IN) {
            li.errorHandler.log("phone.makeCall(): phone in MODE_IN: no outgoing calls allowed.");
            return false;
        }
        options = this.defaults(options, {});
        var opt = {
            number:     null,
            protocol:   'sip:',
            userdata:   null
        };
        jQuery.extend(opt, options);
        if (null === opt.number) {
            throw li.errorType.PHONE_MAKECALL_NUMBERREQUIRED;
        }
        var number = opt.protocol + opt.number + '@' + this.account.host + ':' + this.account.port;
        if (false !== this.options.forceOutgoingNumber) {
            number = this.options.forceOutgoingNumber;
        }
        var id = this.getQtHandler().makeCall(number);
        li.errorHandler.log("phone.makeCall("+number+"): id="+id);
        if (id < 0) {
            throw li.errorType.PHONE_MAKECALL_FAILED;
        }
        var c = new li.Phone.Call({ id: id, phone: this }, { type: li.Phone.Call.TYPE_OUTGOING, number: number, userdata: opt.userdata });
        this.trigger('onMakeCall', { outgoingCall: c });

        var calllist = this.getQtHandler().getActiveCallList();

        return c;
    },
    /**
     * Hangs up all active calls.
     * @param remove    [optional] if true, this.removeAll() is called (default: true).
     */
    hangUpAll: function(remove) {
        li.errorHandler.log("phone.hangUpAll()");
        jQuery.each(this.calls, function(i, callObj) {
            if (callObj) {
                callObj.removeOnClose = callObj.defaults(remove, callObj.removeOnClose);
            }
        });
        this.getQtHandler().hangupAll();
    },
    /**
     * Calls {@link li.Phone.Call#muteSound} method of all calls.
     * @param {boolean} mute    [optional] defaults to true
     * @return this
     */
    muteAllSound: function(mute) {
        mute = this.defaults(mute, true);
        jQuery.each(this.calls, function(i, callObj) {
            if (callObj) {
                callObj.muteSound(mute);
            }
        });
        return this;
    },
    /**
     * Calls {@link li.Phone.Call#muteMicro} method of all calls.
     * @param {boolean} mute    [optional] defaults to true
     * @return this
     */
    muteAllMicro: function(mute) {
        mute = this.defaults(mute, true);
        jQuery.each(this.calls, function(i, callObj) {
            if (callObj) {
                callObj.muteMicro(mute);
            }
        });
        return this;
    },
    /**
     * Sets phone sound status (if true, sound is muted)
     * @param {boolean} mute    [optional] defaults to true
     * @return this
     */
    mutePhoneSound: function(mute) {
        this.options.mutePhoneSound = this.defaults(mute, true);
        this.getQtHandler().muteSound(this.options.mutePhoneSound);
        return this;
    },
    /**
     * Sets phone micro status (if true, microphone is muted)
     * @param {boolean} mute    [optional] defaults to true
     * @return this
     */
    mutePhoneMicro: function(mute) {
        this.options.mutePhoneMicro = this.defaults(mute, true);
        this.getQtHandler().muteMicrophone(this.options.mutePhoneMicro);
        return this;
    },
    /**
     * Get phones sound status.
     * @return {boolean} is phone sound muted?
     */
    isMutePhoneSound: function() {
        return this.options.mutePhoneSound;
    },
    /**
     * Get phones mute status.
     * @return {boolean} is phone micro muted?
     */
    isMutePhoneMicro: function() {
        return this.options.mutePhoneMicro;
    },
    /**
     * Set phone sound level
     * @param {int} level   0...255
     * @return this
     */
    setSoundLevel: function(level) {
        this.options.soundLevel = this.defaults(level, 255);
        this.getQtHandler().setSoundLevel(this.options.soundLevel);
        return this;
    },
    /**
     * Set phone micro level
     * @param {int} level   0...255
     * @return this
     */
    setMicroLevel: function(level) {
        this.options.microLevel = this.defaults(level, 255);
        this.getQtHandler().setMicrophoneLevel(this.options.microLevel);
        return this;
    },
    /**
     * Get phone sound level
     * @return {int} 0...255
     */
    getSoundLevel: function() {
        return this.options.soundLevel;
    },
    /**
     * Get phone micro level
     * @return {int} 0...255
     */
    getMicroLevel: function() {
        return this.options.microLevel;
    },
    
    /**
     * Set codec priority
     * @param {string} codec (e.g. "PCMA" or "PCMA/8000" or "PCMA/8000/1" for codec G.711a)
     * @param {int} priority 0...254
     * @return this
     */
    setCodecPriority: function(codec, priority) {
        this.getQtHandler().setCodecPriority(codec, priority);
        return this;
    },
    /**
     * Get list of codecs
     * @return List of codecs (codecName: priority)
     */
    getCodecs: function() {
        return this.getQtHandler().getCodecPriorities();
    },

    /**
     * Select or change sound device
     * @param input Device ID of the capture device.
     * @param output Device ID of the playback device.
     */
    setSoundDevice: function(input, output) {
        this.getQtHandler().setSoundDevice(input, output);
    },

    /**
     * Get applications error log data
     * @return {array} list of error log objects
     */
    getErrorLog: function() {
        return this.getQtHandler().getErrorLogData();
    },
    /**
     * Delete applications error log data
     */
    clearErrorLog: function() {
        this.getQtHandler().deleteErrorLogFile();
    },
    /**
     * Sets log level of the application
     * @param {integer} status      li.Phone.LOG_STATUS_*
     */
    setLogLevel: function(level) {
        this.getQtHandler().setLogLevel(level);
    },
    /**
     * Retrieve list of log files
     * @return {array} array of log file names (strings)
     */
    getLogFileList: function() {
        return this.getQtHandler().getLogFileList();
    },
    /**
     * Retrieve content of a log file (has to be in log file list, see {@link li.Phone#getLogFileList})
     * @param {string} filename  name of the log file
     * @return {string} content
     */
    getLogFileContent: function(filename) {
        return this.getQtHandler().getLogFileContent(filename);
    },
    /**
     * Delete log file (has to be in log file list, see {@link li.Phone#getLogFileList})
     * @param {string} filename  name of the log file
     */
    deleteLogFile: function(filename) {
        this.getQtHandler().deleteLogFile(filename);
    }
};
li.inherit(li.Phone, li.BaseObject);



/**
 * Manages a call in the phone application.
 * <pre>
 *  Events:                  Parameter (data)
 *      onAccept            { id: call id }     when callee accepted: same as 'onCallAccept' (see {@link li.Phone})
 *      onClose             { id: call id }     when destination closed call: same as 'onCallClose' (see {@link li.Phone})
 *      onUpdate            {}                  after update()
 * </pre>
 * @class Class that represents a call in the phone application.
 * @augments li.BaseObject
 * @param {Object} options  <pre> {
 *      id:             call id
 *      phone:          reference to a li.Phone object
 *      removeOnClose:  should the call be removed from the phone, when its closed (e.g. hangup)
 *  } </pre>
 * @param {Object} data     see {@link li.Phone.Call#update}
 * @throws {li.errorType}.PHONE_CALL_ERROR
 * @property {integer} id               call id
 * @property {boolean} removeOnClose    Should the call be removed from the phone when it's closed (hangup, etc.)?
 * @property {Object} data              see {@link li.Phone.Call#update}
 * @property {Date} callTime            datetime on which this object was created.
 * @property {Date} acceptTime          datetime on which the call was accepted.
 * @property {Date} closeTime           datetime on which the call was terminated.
 * @property {li.Phone} phone           Phone object
 */
li.Phone.Call = function(options, data) {
    jQuery.extend(this, options);
    this.data = {
        type:               li.Phone.Call.TYPE_UNKNOWN,
        status:             li.Phone.Call.STATUS_UNKNOWN,

        number:             null,        // number
        callee:             null,        // number/identifier, that has been dialed (if incoming)
        state:              0,           // call state
        stateText:          '',          // call state text
        lastStatus:         0,           // last received status code
        duration:           -1,          // call duration (in seconds)

        //TODO: currently not used!
        muteSound:          false,       // transfer from destination deactivated
        muteMicro:          false,       // transfer to destination deactivated

        callTime:           0,
        acceptTime:         0,
        closeTime:          0,

        userdata:           null
    };
    if (null === this.phone) {
        throw li.errorType.PHONE_CALL_ERROR;
    }
    this.update(data);

    this.callTime = Date.now();
    this.data.status = li.Phone.Call.STATUS_RINGING;
    this.phone.addCall(this);

    this.addListener('onAccept', function(data) {
        li.errorHandler.log("phone.calls["+this.id+"].onAccept: data.id: "+data.id);
        this.acceptTime = Date.now();
        this.data.status = li.Phone.Call.STATUS_ACCEPTED;
    });
    this.addListener('onClose', function(data) {
        li.errorHandler.log("phone.calls["+this.id+"].onClose: data.id: "+data.id);
        this.closeTime = Date.now();
        this.data.status = li.Phone.Call.STATUS_CLOSED;
        if (true === this.removeOnClose) {
            this.phone.removeCall(this);
        }
    });
};
jQuery.extend(li.Phone.Call, {
    TYPE_UNKNOWN:       -1,
    TYPE_INCOMING:      0,
    TYPE_OUTGOING:      1,

    STATUS_UNKNOWN:     -1,
    STATUS_RINGING:     0,
    STATUS_ACCEPTED:    1,
    STATUS_CLOSED:      2,
    STATUS_ERROR:       3,
    
    SIP_STATE_NULL:         0,	    // Before INVITE is sent or received
    SIP_STATE_CALLING:      1,	    // After INVITE is sent
    SIP_STATE_INCOMING:     2,	    // After INVITE is received
    SIP_STATE_EARLY:        3,	    // After response with To tag
    SIP_STATE_CONNECTING:   4,	    // After 2xx is sent/received
    SIP_STATE_CONFIRMED:    5,	    // After ACK is sent/received
    SIP_STATE_DISCONNECTED: 6       // Session is terminated
});
li.Phone.Call.prototype = {
    id:                 -1,
    phone:              null,

    removeOnClose:      true,

    data:               null,

    callTime:           null,       // Date object;
    acceptTime:         null,
    closeTime:          null,

    /**
     * Update call data.
     *  Triggers 'onUpdate'.
     * @param data <pre> {
     *               type:          -1: unkown, 0: incoming, 1: outgoing
     *               status:        -1: n/a, 0: waiting/ringing, 1: accepted, 2: closed, 3: closed with error
     *               number:        remove uri
     *               state:         call state
     *               stateText:     call state text
     *               lastStatus:    last received status code
     *               duration:      call duration (in seconds)
     *               muteSound:     transfer from call destination deactivated ("sound out")
     *               muteMicro:     transfer to call destination deactivated ("micro out")
     *               callTime:      timestamp in milliseconds (from li.Phone.Call.callTime Date object)
     *               acceptTime:    timestamp in milliseconds (from li.Phone.Call.acceptTime Date object)
     *               closeTime:     timestamp in milliseconds (from li.Phone.Call.closeTime Date object)
     *               userdata:      user specific data
     *             } </pre>
     * @return this
     */
    update: function(data) {
        this.setData(data);

        if (typeof data.muteSound !== 'undefined' && true === data.muteSound) { this.muteSound(); }
        if (typeof data.muteMicro !== 'undefined' && true === data.muteMicro) { this.muteMicro(); }
        if (typeof data.callTime !== 'undefined'   && 0 !== data.callTime)   { this.callTime   = new Date(data.callTime); }
        if (typeof data.acceptTime !== 'undefined' && 0 !== data.acceptTime) { this.acceptTime = new Date(data.acceptTime); }
        if (typeof data.closeTime !== 'undefined'  && 0 !== data.closeTime)  { this.closeTime  = new Date(data.closeTime); }
        this.trigger('onUpdate');
        return this;
    },
    /**
     * Accepts an incoming call.
     * @return this
     */
    accept: function() {
        this.acceptTime = Date.now();
        this.data.status = li.Phone.Call.STATUS_ACCEPTED;
        this.phone.getQtHandler().callAccept(this.id);
        return this;
    },
    /**
     * Hangs up a call.
     * @param {boolean} remove    [default: this._removeOnClose] if true, the call will be removed from the phone
     * @return this
     */
    hangUp: function(remove) {
        li.errorHandler.log("phone.calls["+this.id+"].hangUp()");
        this.removeOnClose = this.defaults(remove, this.removeOnClose);
        this.phone.getQtHandler().hangup(this.id);
        return this;
    },
    /**
     * Check, if call is ringing.
     * @return {boolean}
     */
    isRinging: function() {
        return (this.data.status === li.Phone.Call.STATUS_RINGING);
    },
    /**
     * Check, if call is active/accepted.
     * @return {boolean}
     */
    isAccepted: function() {
        return (this.data.status === li.Phone.Call.STATUS_ACCEPTED);
    },
    /**
     * Check, if call is closed.
     * @return {boolean}
     */
    isClosed: function() {
        return (this.data.status === li.Phone.Call.STATUS_CLOSED);
    },
    /**
     * Check, if call is an incoming call.
     * @return {boolean} true, if this call is an incoming call.
     */
    isIncoming: function() {
        return (this.data.type === li.Phone.Call.TYPE_INCOMING) ? true : false;
    },
    /**
     * Check, if call is an outgoing call.
     * @return {boolean} true, if this call is an outgoing call.
     */
    isOutgoing: function() {
        return (this.data.type === li.Phone.Call.TYPE_OUTGOING) ? true : false;
    },
    /**
     * Get call status.
     * @return {integer} call status li.Phone.Call.STATUS_*
     */
    getStatus: function() {
        return this.data.status;
    },
    /**
     * Mute or unmute sound.
     * @param {boolean} mute  [default: true] if true, deactivate transfer from call destination ("sound out")
     */
    muteSound: function(mute) {
        this.data.muteSound = this.defaults(mute, true);
        //TODO:
        //this.phone.getQtHandler().muteCallSound(this.id, this.data.muteSound);
    },
    /**
     * Check if sound is muted.
     * @return boolean true, if transfer from call destination is deactivated ("sound out")
     */
    isMuteSound: function() {
        return this.data.muteSound;
    },
    /**
     * Mute or unmute micro.
     * @param boolean mute  [default: true] if true, deactivate transfer to call destination ("micro out")
     */
    muteMicro: function(mute) {
        this.data.muteMicro = this.defaults(mute, true);
        //TODO:
        //this.phone.getQtHandler().muteCallMicrophone(this.id, this.data.muteMicro);
    },
    /**
     * Check if micro is muted.
     * @return boolean true, if transfer to call destination is deactivated ("micro out")
     */
    isMuteMicro: function() {
        return this.data.muteMicro;
    },
    //TODO: Implement setSoundLevel and setMicroLevel for calls

    /**
     * Get time from call to accept.
     * @param {boolean} millisecs   [optional] if true, returns time in milliseconds (defaults to false)
     * @return {integer} time in seconds [optional: milliseconds] from call to accept
     */
    getRingTime: function(millisecs) {
        millisecs = this.defaults(millisecs, false);
        if (null === this.callTime) {
            return 0;
        }
        var to = (null === this.acceptTime) ? ((null === this.closeTime) ? Date.now() : this.closeTime) : this.acceptTime;
        return millisecs ? to.getTime() - this.callTime.getTime() :
                           Math.round((to.getTime() - this.callTime.getTime()) * 0.001);
    },
    /**
     * Get time from accept to close.
     * @param {boolean} millisecs   [optional] if true, returns time in milliseconds (defaults to false)
     * @return {integer} time in seconds [optional: milliseconds] from accept to close
     */
    getCallTime: function(millisecs) {
        millisecs = this.defaults(millisecs, false);
        if (null === this.acceptTime) {
            return 0;
        }
        var to = (null === this.closeTime) ? Date.now() : this.closeTime;
        return millisecs ? to.getTime() - this.acceptTime.getTime() :
                           Math.round((to.getTime() - this.acceptTime.getTime()) * 0.001);
    },
    /**
     * Get time from call to close.
     * @param {boolean} millisecs   [optional] if true, returns time in milliseconds (defaults to false)
     * @return {integer} time in seconds [optional: milliseconds] from call to close
     */
    getTime: function(millisecs) {
        millisecs = this.defaults(millisecs, false);
        if (null === this.callTime) {
            return 0;
        }
        var to = (null === this.closeTime) ? Date.now() : this.closeTime;
        return millisecs ? to.getTime() - this.callTime.getTime() :
                           Math.round((to.getTime() - this.callTime.getTime()) * 0.001);
    },

    /**
     * Set data (via deep copy).
     * @param {Object} data   see {@link li.Phone.Call#update}
     * @return this
     */
    setData: function(data) {
        data = this.defaults(data, {});
        jQuery.extend(true, this.data, data);

        if (data.userdata) {
            this.phone.getQtHandler().setCallUserData(this.id, JSON.stringify(this.data.userdata));
            var getData = jQuery.parseJSON(this.phone.getQtHandler().getCallUserData(this.id));
        }
        return this;
    },
    /**
     * Get data.
     * @return {Object} see {@link li.Phone.Call#update}
     */
    getData: function() {
        if (null !== this.callTime)   { this.data.callTime   = this.callTime.getTime(); }
        if (null !== this.acceptTime) { this.data.acceptTime = this.acceptTime.getTime(); }
        if (null !== this.closeTime)  { this.data.closeTime  = this.closeTime.getTime(); }
        return this.data;
    },
    /**
     * Get phone number.
     * @return {string} number
     */
    getNumber: function() {
        var match = /<?(.+):(.+)@(.+)>?/i.exec(this.data.number);
        if (!match || match.length < 3) {
            return this.data.number;
        }
        return match[2];
    },
    /**
     * Get full number.
     * @return {string} number
     */
    getFullNumber: function() {
        return this.data.number;
    },
    /**
     * Get the number/identifier, that has been dialed (only on incoming calls).
     * @return {string}
     */
    getCallee: function() {
        return this.data.callee;
    },
    /**
     * Checks, if no incoming number has been sent
     * @return {boolean}
     */
    isAnonymous: function() {
        return (this.getNumber() === 'anonymous');
    },
    /**
     * Get number.
     * @return {object} { number: 'number', protocol: 'sip', host: 'ip', port: 5060, info: 'additional information' }
     */
    getNumberDetail: function() {
        var match = /<?(.+):(.+)@(.+)[:(.+)][;(.+)]>?/i.exec(this.data.number), data = {};
        if (!match) {
            return data;
        }
        data.protocol = match[1];
        data.number = match[2];
        data.host = match[3];
        data.port = match[4];
        data.info = match[5];
        return data;
    },
    /**
     * Update calls SIP status
     * @param {integer} lastStatus  see li.Phone.SIP_SC_*
     */
    updateLastStatus: function(lastStatus) {
        this.data.lastStatus = lastStatus;
    },
    /**
     * Set user specific data.
     * @param {mixed} userdata
     * @param {boolean} extend      Should the userdata be extended (Default), otherwise it will be replaced
     * @param {boolean} qthandler   Should the userdata be sent to the QtHandler? (default: true)
     */
    setUserData: function(userdata, extend, qthandler) {
        extend = this.defaults(extend, false);
        qthandler = this.defaults(qthandler, true);
        if (extend) {
            jQuery.extend(this.data.userdata, userdata);
        } else {
            this.data.userdata = userdata;
        }
        if (qthandler) {
            this.phone.getQtHandler().setCallUserData(this.id, JSON.stringify(this.data.userdata));
            var data = jQuery.parseJSON(this.phone.getQtHandler().getCallUserData(this.id));
        }
    },
    /**
     * Get user specific data.
     * @return {mixed}
     */
    getUserData: function() {
        return this.data.userdata;
    }
};
li.inherit(li.Phone.Call, li.BaseObject);


/**
 * The phone application uses this handlers callback methods when phone events occur.
 * @class Phone handler: callback interface for the phone application.
 * @property {li.Phone} phone   Phone object.
 */
li.Phone.Handler = function(phone) {
    this.phone = phone;
};
li.Phone.Handler.prototype = {
    phone: null,

    /**
     * An incoming call with id and number has been detected.
     *  Triggers li.Phone.'onIncomingCall' with { incomingCall: {@link li.Phone.Call} };
     *  li.Phone.'onError' with { message: string, exception: e }.
     * @param {integer} call_id
     * @param {string} number       incoming number
     * @param {string} callee       the number/identifier, that has been called
     * @return {boolean} true, if successful
     */
    incomingCall: function(call_id, number, callee) {
        li.errorHandler.log("phone.Handler.incomingCall("+call_id+",'"+number+"','"+callee+"')");
        var opt = {
                id:     call_id,
                phone:  this.phone
            },
            data = {
                type:   li.Phone.Call.TYPE_INCOMING,
                number: number,
                callee: callee
            };
        try {
            var c = new li.Phone.Call(opt, data);
            //TODO: this should be handled by the pjsip wrapper!
            if (this.phone.getMode() === li.Phone.MODE_OUT) {
                li.errorHandler.log("phone.Handler: phone in MODE_OUT: hangup("+call_id+")");
                this.phone.getQtHandler().hangup(call_id);
                return false;
            }
            this.phone.trigger('onIncomingCall', { incomingCall: c } );
        } catch(e) {
            this.phone.trigger('onError', { message: "li.Phone.Handler.incomingCall("+call_id+",'"+number+"','"+callee+"'): exception: '"+e+"'", exception: e } );
            return false;
        }
        return true;
    },
    /**
     * A log object/message has been sent.
     *  Triggers li.Phone.'onLogMessage' with obj.
     * @param {Object} obj      log object; see {@link li.Phone#sendLogMessage}
     */
    logMessage: function(obj) {
        this.phone.trigger('onLogMessage', obj);
    },
    /**
     * The call state has been updated.
     *  Triggers li.Phone.'onCallAccept' with { id: call_id };
     *  li.Phone.'onCallClose' with { id: call_id };
     *  li.Phone.'onError' with { message: string, exception: e }.
     * @param {integer} call_id
     * @param {integer} state        see li.Phone.Call.SIP_STATE_*
     * @param {integer} lastStatus  calls last sip status; see li.Phone.SIP_SC_*
     */
    callStateChanged: function(call_id, state, lastStatus) {
        li.errorHandler.log("phone.Handler.callStateChanged("+call_id+", "+state+"): phone.hasCallById=" + this.phone.hasCallById(call_id));
        //TODO: state 1 is currently ignored, because it is sent immediatly after QtHandler.makeCall() is called and
        // the call is still not in the phones calllist (error when updating via id => call not found!)
        if (state === li.Phone.Call.SIP_STATE_CALLING) {
            return;
        }
        if (!this.phone.hasCallById(call_id)) {
            this.phone.trigger('onError', { message: "li.Phone.Handler.callStateChanged("+call_id+","+state+","+lastStatus+"): Call with id '"+call_id+"' does not exist." } );
            return;
        }
        this.phone.getCallById(call_id).updateLastStatus(lastStatus);
        switch(state) {
            case li.Phone.Call.SIP_STATE_CONFIRMED:         // call accepted
                this.phone.trigger('onCallAccept', { id: call_id } );
                break;
            case li.Phone.Call.SIP_STATE_DISCONNECTED:      // call hangup
                this.phone.trigger('onCallClose', { id: call_id } );
                break;
        }
    },
    /**
     * The account state has been updated.
     *  Triggers li.Phone.'onAccountState' with { state: state };
     * @param {integer} state
     */
    accountStateChanged: function(state) {
        this.phone.trigger('onAccountState', { state: state } );
    },
    /**
     * The sound level has changed
     *  Triggers li.Phone.'onSoundLevelChanged' with level;
     * @param {integer} level       level, that was triggered (0..255)
     */
    soundLevel: function(level) {
        this.phone.trigger('onSoundLevelChanged', level);
    },
    /**
     * The microphone level has changed
     *  Triggers li.Phone.'onMicroLevelChanged' with level;
     * @param {integer} level       level, that was triggered (0..255)
     */
    microphoneLevel: function(level) {
        this.phone.trigger('onMicroLevelChanged', level);
    },

    /**
     * The application requests an url that should be printed (e.g. CTRL-P was used)
     *  Triggers li.Phone.'onPrintUrl'
     * @return {string} url
     */
    getPrintUrl: function() {
        var urls = this.phone.trigger('onPrintUrl');
        if (urls && urls.length > 0) {
            return urls[0];
        }
        return '';
    }
};

/**
 * li.errorType extend Phone Error Types
 */
jQuery.extend(li.errorType, {
    PHONE_FAILED:                       2000,
    PHONE_ERROR:                        2001,
    PHONE_DEBUG:                        2002,
    PHONE_REGISTRATION_FAILED:          2010,
    PHONE_REGISTRATION_ERROR:           2011,
    PHONE_MAKECALL_FAILED:              2020,
    PHONE_MAKECALL_NUMBERREQUIRED:      2021,
    PHONE_CALL_ERROR:                   2030
});

}(window.li));