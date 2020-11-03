[top]
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
crossing : 0:0:3:100
capacity : 5
input : disable
output : enable

[intersection_controller]

