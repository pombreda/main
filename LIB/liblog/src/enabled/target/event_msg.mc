; /*
;   HEADER SECTION
;  */
SeverityNames=(Success=0x0:STATUS_SEVERITY_SUCCESS
               Informational=0x1:STATUS_SEVERITY_INFORMATIONAL
               Warning=0x2:STATUS_SEVERITY_WARNING
               Error=0x3:STATUS_SEVERITY_ERROR
              )
;
;
FacilityNames=(System=0x0:FACILITY_SYSTEM
               Runtime=0x2:FACILITY_RUNTUME
               Stubs=0x3:FACILITY_STUBS
               Io=0x4:FACILITY_IO_ERROR_CODE
              )
;
; /*
;    MESSAGE DEFINITION SECTION
;  */
;
; /* Categories */
;

MessageIdTypedef=WORD

MessageId=0x1
SymbolicName=EV_CATEGORY_1
Language=English
Category 1
.


; /* Messages */

MessageIdTypedef=DWORD

MessageId=0x100
Severity=Success
Facility=Runtime
SymbolicName=EV_MSG_STRING
Language=English
%1
.
MessageId=0x200
Severity=Success
Facility=Runtime
SymbolicName=EV_MSG_SUCCESS
Language=English
String_EV_MSG_SUCCESS %1
.
MessageId=0x300
Severity=Informational
Facility=Runtime
SymbolicName=EV_MSG_INFO
Language=English
String_EV_MSG_INFO %1
.
MessageId=0x400
Severity=Warning
Facility=Runtime
SymbolicName=EV_MSG_WARNING
Language=English
String_EV_MSG_WARNING %1
.
MessageId=0x500
Severity=Error
Facility=Runtime
SymbolicName=EV_MSG_ERROR
Language=English
String_EV_MSG_ERROR %1
.


; /* Insert string parameters */

MessageId=1000
Severity=Success
Facility=System
SymbolicName=PARAM_1
Language=English
Parameter1
.
