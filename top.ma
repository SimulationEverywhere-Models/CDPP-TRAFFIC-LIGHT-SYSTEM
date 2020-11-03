[top]
components : light_group_controller_ab@LightGroupController
components : light_group_controller_cd@LightGroupController
components : traffic_light_a
components : traffic_light_b
components : traffic_light_c
components : traffic_light_d

out : light_a throughput_a curlen_a avelen_a
Link : lights@light_group_controller_ab signal@traffic_light_a
Link : light@traffic_light_a light_a
Link : throughput@traffic_light_a throughput_a
Link : curlength@traffic_light_a curlen_a
Link : avelength@traffic_light_a avelen_a

out : light_b throughput_b curlen_b avelen_b
Link : lights@light_group_controller_ab signal@traffic_light_b
Link : light@traffic_light_b light_b
Link : throughput@traffic_light_b throughput_b
Link : curlength@traffic_light_b curlen_b
Link : avelength@traffic_light_b avelen_b

out : light_c throughput_c curlen_c avelen_c
Link : lights@light_group_controller_cd signal@traffic_light_c
Link : light@traffic_light_c light_c
Link : throughput@traffic_light_c throughput_c
Link : curlength@traffic_light_c curlen_c
Link : avelength@traffic_light_c avelen_c

out : light_d throughput_d curlen_d avelen_d
Link : lights@light_group_controller_cd signal@traffic_light_d
Link : light@traffic_light_d light_d
Link : throughput@traffic_light_d throughput_d
Link : curlength@traffic_light_d curlen_d
Link : avelength@traffic_light_d avelen_d

Link : standby_out@light_group_controller_cd standby_in@light_group_controller_ab
Link : standby_out@light_group_controller_ab standby_in@light_group_controller_cd

[light_group_controller_ab]
green : 0:0:30:0
yellow : 0:0:3:0
standby : 0:0:2:0
state : standby

[light_group_controller_cd]
green : 0:0:45:0
yellow : 0:0:3:0
standby : 0:0:2:0
state : red




[traffic_light_a]
components : collector_a@TrafficCollector
components : traffic_light_controller_a@TrafficLightController
components : lane_a
in : signal
out :  light throughput curlength avelength
Link : signal in@traffic_light_controller_a
Link : car_out@lane_a leaved@collector_a
Link : car_in@lane_a entered@collector_a
Link : out@traffic_light_controller_a stop@lane_a
Link : light@traffic_light_controller_a light
Link : throughput@collector_a throughput
Link : curQueueLength@collector_a curlength
Link : aveQueueLength@collector_a avelength
[traffic_light_controller_a]
[collector_a]
frequence : 0:0:30:000
[lane_a]
components : generator_a@Generator lane_queue_a@GenericQueue intersection_a
in : stop
out : car_out car_in
Link : out@intersection_a car_out
Link : out@generator_a car_in
Link : stop stop@intersection_a
Link : out@generator_a in@lane_queue_a
Link : out@lane_queue_a in@intersection_a
Link : next@intersection_a done@lane_queue_a
Link : block@intersection_a stop_out@lane_queue_a
[generator_a]
distribution : normal
mean : 3
deviation : 1
initial : 1
increment : 1
[lane_queue_a]
preparation : 0:0:1:000
capacity : 0
input : enable
output : enable
[intersection_a]
components : intersection_queue_a@IntersectionQueue
components : intersection_controller_a@IntersectionController
in : stop in
out : next block out
Link : in in@intersection_queue_a
Link : stop stop_in@intersection_queue_a
Link : out@intersection_queue_a in@intersection_controller_a
Link : out@intersection_controller_a done@intersection_queue_a
Link : busy@intersection_controller_a stop_out@intersection_queue_a
Link : block@intersection_queue_a block
Link : next@intersection_queue_a next
Link : out@intersection_controller_a out
[intersection_queue_a]
crossing : 0:0:3:0
capacity : 2
input : disable
output : enable
[intersection_controller_a]




[traffic_light_b]
components : collector_b@TrafficCollector
components : traffic_light_controller_b@TrafficLightController
components : lane_b
in : signal
out :  light throughput curlength avelength
Link : signal in@traffic_light_controller_b
Link : car_out@lane_b leaved@collector_b
Link : car_in@lane_b entered@collector_b
Link : out@traffic_light_controller_b stop@lane_b
Link : light@traffic_light_controller_b light
Link : throughput@collector_b throughput
Link : curQueueLength@collector_b curlength
Link : aveQueueLength@collector_b avelength
[traffic_light_controller_b]
[collector_b]
frequence : 0:0:30:000
[lane_b]
components : generator_b@Generator lane_queue_b@GenericQueue intersection_b
in : stop
out : car_out car_in
Link : out@intersection_b car_out
Link : out@generator_b car_in
Link : stop stop@intersection_b
Link : out@generator_b in@lane_queue_b
Link : out@lane_queue_b in@intersection_b
Link : next@intersection_b done@lane_queue_b
Link : block@intersection_b stop_out@lane_queue_b
[generator_b]
distribution : normal
mean : 3
deviation : 1
initial : 1
increment : 1
[lane_queue_b]
preparation : 0:0:1:000
capacity : 0
input : enable
output : enable
[intersection_b]
components : intersection_queue_b@IntersectionQueue
components : intersection_controller_b@IntersectionController
in : stop in
out : next block out
Link : in in@intersection_queue_b
Link : stop stop_in@intersection_queue_b
Link : out@intersection_queue_b in@intersection_controller_b
Link : out@intersection_controller_b done@intersection_queue_b
Link : busy@intersection_controller_b stop_out@intersection_queue_b
Link : block@intersection_queue_b block
Link : next@intersection_queue_b next
Link : out@intersection_controller_b out
[intersection_queue_b]
crossing : 0:0:3:0
capacity : 2
input : disable
output : enable
[intersection_controller_b]




[traffic_light_c]
components : collector_c@TrafficCollector
components : traffic_light_controller_c@TrafficLightController
components : lane_c
in : signal
out :  light throughput curlength avelength
Link : signal in@traffic_light_controller_c
Link : car_out@lane_c leaved@collector_c
Link : car_in@lane_c entered@collector_c
Link : out@traffic_light_controller_c stop@lane_c
Link : light@traffic_light_controller_c light
Link : throughput@collector_c throughput
Link : curQueueLength@collector_c curlength
Link : aveQueueLength@collector_c avelength
[traffic_light_controller_c]
[collector_c]
frequence : 0:0:30:000
[lane_c]
components : generator_c@Generator lane_queue_c@GenericQueue intersection_c
in : stop
out : car_out car_in
Link : out@intersection_c car_out
Link : out@generator_c car_in
Link : stop stop@intersection_c
Link : out@generator_c in@lane_queue_c
Link : out@lane_queue_c in@intersection_c
Link : next@intersection_c done@lane_queue_c
Link : block@intersection_c stop_out@lane_queue_c
[generator_c]
distribution : normal
mean : 3
deviation : 1
initial : 1
increment : 1
[lane_queue_c]
preparation : 0:0:1:000
capacity : 0
input : enable
output : enable
[intersection_c]
components : intersection_queue_c@IntersectionQueue
components : intersection_controller_c@IntersectionController
in : stop in
out : next block out
Link : in in@intersection_queue_c
Link : stop stop_in@intersection_queue_c
Link : out@intersection_queue_c in@intersection_controller_c
Link : out@intersection_controller_c done@intersection_queue_c
Link : busy@intersection_controller_c stop_out@intersection_queue_c
Link : block@intersection_queue_c block
Link : next@intersection_queue_c next
Link : out@intersection_controller_c out
[intersection_queue_c]
crossing : 0:0:3:0
capacity : 2
input : disable
output : enable
[intersection_controller_c]




[traffic_light_d]
components : collector_d@TrafficCollector
components : traffic_light_controller_d@TrafficLightController
components : lane_d
in : signal
out :  light throughput curlength avelength
Link : signal in@traffic_light_controller_d
Link : car_out@lane_d leaved@collector_d
Link : car_in@lane_d entered@collector_d
Link : out@traffic_light_controller_d stop@lane_d
Link : light@traffic_light_controller_d light
Link : throughput@collector_d throughput
Link : curQueueLength@collector_d curlength
Link : aveQueueLength@collector_d avelength
[traffic_light_controller_d]
[collector_d]
frequence : 0:0:30:000
[lane_d]
components : generator_d@Generator lane_queue_d@GenericQueue intersection_d
in : stop
out : car_out car_in
Link : out@intersection_d car_out
Link : out@generator_d car_in
Link : stop stop@intersection_d
Link : out@generator_d in@lane_queue_d
Link : out@lane_queue_d in@intersection_d
Link : next@intersection_d done@lane_queue_d
Link : block@intersection_d stop_out@lane_queue_d
[generator_d]
distribution : normal
mean : 3
deviation : 1
initial : 1
increment : 1
[lane_queue_d]
preparation : 0:0:1:000
capacity : 0
input : enable
output : enable
[intersection_d]
components : intersection_queue_d@IntersectionQueue
components : intersection_controller_d@IntersectionController
in : stop in
out : next block out
Link : in in@intersection_queue_d
Link : stop stop_in@intersection_queue_d
Link : out@intersection_queue_d in@intersection_controller_d
Link : out@intersection_controller_d done@intersection_queue_d
Link : busy@intersection_controller_d stop_out@intersection_queue_d
Link : block@intersection_queue_d block
Link : next@intersection_queue_d next
Link : out@intersection_controller_d out
[intersection_queue_d]
crossing : 0:0:3:0
capacity : 2
input : disable
output : enable
[intersection_controller_d]

