[top]
components : intersection_queue@IntersectionQueue

in : in done stop_in stop_out
out : out next block
Link : in in@intersection_queue
Link : done done@intersection_queue
Link : stop_in stop_in@intersection_queue
Link : stop_out stop_out@intersection_queue
Link : out@intersection_queue out
Link : block@intersection_queue block
Link : next@intersection_queue next

[intersection_queue]
crossing : 0:0:3:000
# maximum size of the queue, 0 - unlimited
capacity :  3
input : enable
output : enable
