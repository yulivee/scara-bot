# How to ROS with the Arduino

## ROS-Master Setup on Laptop

### Core Services
First start the rosmaster in a terminal
```
roscore
```

Then start rosserial in another terminal
```
rosrun rosserial_python serial_node.py /dev/ttyUSB1 _baud:=57600
```

Now you can monitor the wheel encoder position from the `wheel_encoder_clicks` publisher
```
rostopic echo wheel_encoder_clicks
```

### Subscribers

| Topic        | Message Type    | Purpose                                   | Command                                              |
| :----------- |:---------------:| :---------------------------------------- | :--------------------------------------------------- |
 toggle\_motor | std\_msgs/Empty | Toggle Motors on/off, stop PID Controller | `rostopic pub toggle_motor std_msgs/Empty --once`
 home          | std\_msgs/Empty | Set current position as new zero          | `rostopic pub home std_msgs/Empty --once`
 drive\_dist   | std\_msgs/Int32 | Drive <n> encoder clicks                  | `rostopic pub drive_dist std_msgs/Int32 --once -- <n>`
 drive\_to     | std\_msgs/Int32 | Drive until encoder click <n> is reached  | `rostopic pub drive_to std_msgs/Int32 --once -- <n>`

     
