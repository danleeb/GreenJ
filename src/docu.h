/**
\page pageqtjs QT-JavaScript Communication
This application is designed to have its user interface as a webpage. So you can easily customize the user interface to your wish. The documentation to access the application from your web-page you can find here: \ref JavascriptHandler\n
The following functions have to be implemented in the web-application
\section bsec1 accountStateChanged
The login methode doesn't show if server accept the login. So we need to check the
account state. This function gets called when the account state changes. The most
important callstates are:
- 200 for login is accepted
- 403 for login data are wrong
- 408 for timeout, maybe hostname is wrong
@param state int, the new state of account
\section bsec2 callStateChanged
Callstates are codes that tells the current status of the call, like calling, 
confirmed or disconnect. This function gets called every time a callstate gets changed
@param call int, the id of the call
@param code int, the new state-code
@param status the old state code
\section bsec4 incomingCall
When try to call the user, this function is needed to inform the webpage about
the call. The call_id is needed to handle this call in future, like answering it
or hanging up.
@param call int, the id of the new call
@param sip_url string, the number of the new call
\section bsec6 getPrintUrl
This function is called to ask the webapplication for an url to a printable page
@return string the url
\section bsec7 logMessage
For reporting errors or debug messages to the user, this function is used.
You can deside, what to do, just printing the message or sending it to a server.
@param json a log object with following elements:
- time in format "dd.MM.yyyy hh:mm:ss"
- status, the application error code (see \ref LogInfo)
- domain, the location the error is thrown
- code, if available an additional error code
- message, a fulltext error message
\section bsec8 soundLevel
This function gets called when sound volume gets changed
@param level int, the current sound level
\section bsec9 microphoneLevel
This function gets called when microphone volume gets changed
@param level int, the current microphone level
 */

//----------------------------------------------------------------------

/**
\page pagephoneapi The Phone Protocol
\par
This application is designed to be able to replace the phone protocol easily.
We have added a sip-implementation using the pjsip library.
\par
If you want to use a different sip-library or an other voip-protocol
you have to implement a protocol class extended on PhoneApi.
\par
To register your phone protocol to the system, you have to edit the gui.cpp file.
You will find following initialization of class member phone_:
\code
phone_(new SipPhone)
\endcode
Here you have to change SipPhone with you own protocol class
 */

//----------------------------------------------------------------------

/**
\page pageconfig The Config File
The config file can be found at $HOME"/.lorifon/settings.conf"

The following vars can be interessting:
- log_level, tells the level needed for logging messages
- app_minimizeable, allows window to get minimized
- app_maximizeable, allows window to get maximized
- app_fullscreenable, allows window to get fullscreen
- app_resizeable, allows window to get resized

- url, location of the web-page

\section Default Config
In config_file_handler.cpp you can find the default config.
It is part of the init-methode:
\code
if (!QFile::exists(file_name_))
{
    // create basic config
    my_settings_.beginGroup("application");
    my_settings_.setValue("configversion","1");
    my_settings_.setValue("appversion","1.0");
    my_settings_.setValue("appname","Lorifon");
    my_settings_.setValue("developer","Lorem Ipsum");
    my_settings_.setValue("app_minimizeable","true");
    my_settings_.setValue("app_maximizeable","true");
    my_settings_.setValue("app_fullscreenable","true");
    my_settings_.setValue("app_resizeable","true");
    my_settings_.setValue("log_level",LogInfo::STATUS_WARNING);
    my_settings_.endGroup();

    my_settings_.beginGroup("gui");
    my_settings_.setValue("soundfile","ring.wav");
    my_settings_.setValue("sounddialfile","dial_tone.wav");
    my_settings_.endGroup();

    // url to the web-page
    my_settings_.beginGroup("server");
    my_settings_.setValue("url","testphone.html");
    my_settings_.endGroup();
}
\endcode
This creates the config-file the first time you start the application. It can be very 
useful to change to the url to the location of your web-page.
 */
