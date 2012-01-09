/**
 * @package    Javascript
 * @subpackage Phone
 * @copyright  Copyright (c) 2011 Lorem Ipsum Mediengesellschaft m.b.H. (http://www.loremipsum.at)
 *
 * GNU General Public License
 * This file may be used under the terms of the GNU General Public License
 * version 3 as published by the Free Software Foundation and
 * appearing in the file LICENSE.GPL included in the packaging of this file.
 *
 */
 
if (typeof window.qt_handler === 'undefined')
{

/**
 * @namespace QtHandler dummy object.
 */
window.qt_handler = {
    _registered: false,
    
    incomingNumber:     '123456789',
    
    nextid:             5,
    calls:              [],
    sound:              1,
    micro:              1,
    
    phoneHandler:       null,
    
    account: {
        address:        'sip',
        status:         0,
        online_status:  0
    },
    
    registerJsCallbackHandler: function(phoneHandler) {
        eval('this.phoneHandler = ' + phoneHandler);
    },
    
    sendLogMessage: function(obj) {
        jQuery('#dummyLog').text(obj.time + ': (' + obj.status + ') <' + obj.domain + '> [' + obj.code + '] ' + obj.message);
    },
    
    registerToServer: function(host, name, secret) {
        this._registered = true;
        return true;
    },
    unregisterFromServer: function() {
        this._registered = false;
        return true;
    },
    checkAccountStatus: function() {
        return this._registered;
    },
    getAccountInformation: function() {
        return this.account;
    },
    getActiveCallList: function() {
        var list = [], i;
        for (i in this.calls) {
            if (this.calls.hasOwnProperty(i)) {
                var call = this.calls[i];
                list.push({
                    id:          call.id,
                    number:      call.number,
                    info:        call.info,
                    state:       call.state,
                    stateText:   call.stateText,
                    status:      call.status,
                    lastStatus:  call.lastStatus,
                    duration:    call.duration,
                    callTime:    call.callTime,
                    acceptTime:  call.acceptTime,
                    closeTime:   call.closeTime
                });
            }
        }
        return list;
    },
    hangupAll: function() {
        var self = this;
        jQuery.each(this.calls, function(i, c) { self.triggerCallClose(i); } );
        this.calls = [];
        
        jQuery('#dummyTitle').text('Call hangup.');
        jQuery('#dummyAccept, #dummyHangUp').attr('disabled', 'disabled');
        jQuery('#dummyId, #dummyNumber').val('');
        jQuery('#dummyCall').removeAttr('disabled');
    },
    makeCall: function(number) {
        var id = this.newCall(number);
                
        jQuery('#dummyTitle').text('Incoming call...');
        jQuery('#dummyAccept').removeAttr('disabled');
        
        return id;
    },
    hangup: function(id) {
        if (typeof this.calls[id] === 'undefined') { return; }
        
        this.triggerCallClose(id);
    },
    callAccept: function(id) {
        if (typeof this.calls[id] === 'undefined') { return; }
        this.calls[id].state = 0;
        this.calls[id].stateText = '';
        this.calls[id].lastStatus = 200;
        this.calls[id]._intern.acceptTime = Date.now();
        
        this.clearInterval('ringInterval');
        this.clearInterval('callInterval');
        var now = Date.now(),
            t = Math.round((now.getTime() - this.calls[id]._intern.callTime.getTime()) * 0.001);
        jQuery('#dummyRingTime').text(t);
        
        var self = this;
        this.setInterval('callInterval', function() {
                if (typeof self.calls[id] === 'undefined') { return; }
                var now = Date.now(),
                    t = Math.round((now.getTime() - self.calls[id]._intern.acceptTime.getTime()) * 0.001);
                jQuery('#dummyDuration').text(t);
            }, 1000);
                
        jQuery('#dummyTitle').text('Call accepted.');
        jQuery('#dummyAccept').attr('disabled', 'disabled');
        
        this.phoneHandler.callStateChanged(id, 5, this.calls[id].lastStatus); 
    },
    getCallUserData: function(id) {
        if (typeof this.calls[id] === 'undefined') { return; }
        if (typeof this.calls[id].data === 'undefined') {
            return null;
        }
        return this.calls[id].data;
    },
    setCallUserData: function(id, data) {
        if (typeof this.calls[id] === 'undefined') { return; }
        this.calls[id].data = data;
    },    
    muteSound: function(mute) {
        this.sound = (mute ? 0 : 1);
        this.triggerSoundLevel(this.sound);
    },
    muteMicrophone: function(mute) {
        this.micro = (mute ? 0 : 1);
        this.triggerMicrophoneLevel(this.micro);
    },
    getSignalInformation: function() {
        return { sound: this.sound, micro: this.micro };
    },
    newCall: function(number) {
        var id = this.nextid;
        this.nextid++;
        this.calls[id] = {
            id:          id,
            type:        0,
            status:      0,
            
            number:      number,
            info:        'remote_info',
            state:       0,
            stateText:   'state_text',
            lastStatus:  0,
            duration:    0,
            
            soundMuted:  false,
            microMuted:  false,
            
            callTime:    0,
            acceptTime:  0,
            closeTime:   0,
            
            _intern: {
                callTime:    Date.now(),
                acceptTime:  null,
                closeTime:   null
            }
        };
        
        jQuery('#dummyRingTime').text('0');
        jQuery('#dummyDuration').text('0');
        this.clearInterval('ringInterval');
        var self = this;
        this.setInterval('ringInterval', function() {
                if (typeof self.calls[id] === 'undefined') { return; }
                var now = Date.now(),
                    t = Math.round((now.getTime() - self.calls[id]._intern.callTime.getTime()) * 0.001);
                jQuery('#dummyRingTime').text(t);
            }, 1000);
        jQuery('#dummyHangUp').removeAttr('disabled');
        jQuery('#dummyNumber').val(number);
        jQuery('#dummyId').val(id);
        return id;
    },
    
    triggerIncomingCall: function() {
        var id = this.newCall(this.incomingNumber);
        
        jQuery('#dummyTitle').text('Waiting for call accept...');
        jQuery('#dummyHangUp').removeAttr('disabled');
        
        this.phoneHandler.incomingCall(id, this.calls[id].number, '0049123456789');
    },
    triggerDebugMessage: function(msg) {
        this.phoneHandler.debugMessage(msg);
    },
    triggerErrorMessage: function(msg) {
        this.phoneHandler.errorMessage(msg);
    },
    triggerLogMessage: function(obj) {
        this.phoneHandler.logMessage(obj);
    },
    triggerCallAccept: function(id) {
        this.callAccept(id);
    },
    triggerCallClose: function(id) {
        if (typeof this.calls[id] === 'undefined') { return; }
        
        this.phoneHandler.callStateChanged(id, 6, this.calls[id].lastStatus);
        
        this.clearInterval('ringInterval');
        this.clearInterval('callInterval');
        var t, now = Date.now();
        if (null !== this.calls[id]._intern.acceptTime) {
            t =  Math.round((this.calls[id]._intern.acceptTime.getTime() - this.calls[id]._intern.callTime.getTime()) * 0.001);
            jQuery('#dummyRingTime').text(t);
            t = Math.round((now.getTime() - this.calls[id]._intern.acceptTime.getTime()) * 0.001);
            jQuery('#dummyDuration').text(t);
        } else {
            t = Math.round((now.getTime() - this.calls[id]._intern.callTime.getTime()) * 0.001);
            jQuery('#dummyRingTime').text(t);            
        }
                
        jQuery('#dummyTitle').text('Call hangup.');
        jQuery('#dummyAccept, #dummyHangUp').attr('disabled', 'disabled');
        jQuery('#dummyId, #dummyNumber').val('');
        jQuery('#dummyCall').removeAttr('disabled');
        
        delete this.calls[id];
    },
    triggerSoundLevel: function(level) {
        this.phoneHandler.soundLevel(level);
    },
    triggerMicrophoneLevel: function(level) {
        this.phoneHandler.microphoneLevel(level);
    },    
    
    printPage: function(url) {
        jQuery('#dummyLog').text('printPage(' + url + ')');        
    },
    
    getErrorLogData: function() {
        return [/*{ 
            acceptTime: 210866803200000,
            callTime: 1308228097490,
            closeTime: 210866803200000,
            duration: 876229681,
            number: "sip:5050@asterisk.org",
            status: 2,
            lastStatus: 200,
            state: 0,
            type: 1 
        }*/];
    },
    deleteErrorLogFile: function() {
    },
    getLogFileList: function() {
        return [];
    },
    getLogFileContent: function(filename) {
        return '';
    },
    deleteLogFile: function(filename) {
    }    
};
jQuery.extend(window.qt_handler, li.BaseObject.prototype);

jQuery(document).ready(function($) {
    var content = "";
    content += '<button style="float:right;" class="smalltitle" id="dummyClose">x</button>';
    content += '<button style="float:right;" class="smalltitle" id="dummyMin">_</button>';
    content += '<span style="font-weight:bold;" id="dummyTitle">QtPhoneDummy</span><br />';
    content += '<div id="dummyContent">';
    content += '<input type="text" size="2" id="dummyId" value="id" disabled="disabled" />';
    content += '<input type="text" id="dummyNumber" value="number" disabled="disabled" />';
    content += '<button id="dummyAccept">Accept</button>';
    content += '<button id="dummyHangUp">HangUp</button><hr />';
    content += '<button id="dummyCall">Call</button>';
    content += '<input type="text" id="dummyCallNumber" /><hr />';
    content += 'Ring time: <span id="dummyRingTime"></span><br />';
    content += 'Duration: <span id="dummyDuration"></span><br />';
    content += 'Recent log message: <br /><span id="dummyLog" style="font-size:9px;">N/A</span><br />';
    content += '</div>';
    $('<div style="z-index:10101; position:absolute; left:1400px; top:690px; width:230px; background:#f80; padding:5px; -moz-border-radius:4px; border-radius:4px; -moz-box-shadow:3px 3px 3px black; -webkit-box-shadow:3px 3px 3px black; box-shadow:3px 3px 3px black;"></div>')
            .html(content)
            .draggable()
            .appendTo('body');
    $('#dummyCallNumber').val(window.qt_handler.incomingNumber)
        .change(function() {
            window.qt_handler.incomingNumber = jQuery(this).val();
        });
    $('#dummyClose').click(function() {
            $(this).parent().hide();
        });
    $('#dummyMin').click(function() {
            $('#dummyContent').toggle();
        });
    $('#dummyAccept')
        .attr('disabled', 'disabled')
        .click(function() {
            var id = $('#dummyId').val();
            window.qt_handler.triggerCallAccept(id);
        });
    $('#dummyHangUp')
        .attr('disabled', 'disabled')
        .click(function() {
            var id = $('#dummyId').val();
            window.qt_handler.triggerCallClose(id);
        });
    $('#dummyCall')
        .click(function() {
            window.qt_handler.triggerIncomingCall();
        });
});

}