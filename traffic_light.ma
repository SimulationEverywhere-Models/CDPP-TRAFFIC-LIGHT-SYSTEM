[top]
components : collector@TrafficCollector
components : traffic_light_controller@TrafficLightController
components : lane

in : signal
out :  light throughput curlength avelength
Link : signal in@traffic_light_controller
Link : car_out@lane leaved@collector
Link : car_in@lane entered@collector
Link : out@traffic_light_controller stop@lane
Link : light@traffic_light_controller light
Link : throughput@collector throughput
Link : curQueueLength@collector curlength
Link : aveQueueLength@collector avelength

[traffic_light_controller]
[collector]
frequence : 0:0:30:000

[lane]
components : generator@Generator lane_queue@GenericQueue intersection
in : stop
out : car_out car_in
Link : out@intersection car_out
Link : out@generator car_in
Link : stop stop@intersection
Link : out@generator in@lane_queue
Link : out@lane_queue in@intersection
Link : next@intersection done@lane_queue
Link : block@intersection stop_out@lane_queue

[generator]
distribution : normal
mean : 3
deviation : 1
initial : 1
increment : 1

[lane_queue]
preparation : 0:0:1:000
capacity : 0
input : enable
output : enable

[intersection]
components : intersection_queue@IntersectionQueue
components : intersection_controller@IntersectionController
in : stop in
out : next block out
Link : in in@intersection_queue
Link : stop stop_in@intersection_queue
Link : out@intersection_queue in@intersection_controller
Link : out@intersection_controller done@intersection_queue
Link : busy@intersection_controller stop_out@intersection_queue
Link : block@intersection_queue block
Link : next@intersection_queue next
Link : out@intersection_controller out

[intersection_queue]
crossing : 0:0:3:0
capacity : 2
input : disable
output : enable

[intersection_controller]

