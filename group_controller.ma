[top]
components : groupAB@LightGroupController
components : groupCD@LightGroupController

#in : in
out : light_ab light_cd
#Link : in standby_in@groupAB
Link : standby_out@groupCD standby_in@groupAB
Link : standby_out@groupAB standby_in@groupCD
Link : lights@groupAB light_ab
Link : lights@groupCD light_cd

[groupAB]
green : 0:0:30:0
yellow : 0:0:4:0
standby : 0:0:2:0
state : standby

[groupCD]
green : 0:0:45:0
yellow : 0:0:3:0
standby : 0:0:1:0
state : red

