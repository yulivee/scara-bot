# How to ROS with the Arduino

## ROS-Master Setup on Laptop

### Core Services
First start the rosmaster in a terminal
```
roscore
```

Then start rosserial in another terminal
```
rosrun rosserial_python serial_node.py /dev/ttyUSB0 _baud:=57600

Output:
[INFO] [1544547820.900933]: ROS Serial Python Node
[INFO] [1544547820.913043]: Connecting to /dev/ttyUSB0 at 57600 baud
[INFO] [1544547823.067084]: Note: publish buffer size is 512 bytes
[INFO] [1544547823.068397]: Setup publisher on GetPos [std_msgs/Int16MultiArray]
[INFO] [1544547823.075410]: Setup publisher on Debug [std_msgs/String]
[INFO] [1544547823.094746]: Note: subscribe buffer size is 512 bytes
[INFO] [1544547823.095803]: Setup subscriber on DriveTo [std_msgs/Int16MultiArray]
[INFO] [1544547823.104949]: Setup subscriber on Home [std_msgs/Empty]
[INFO] [1544547823.121590]: Setup subscriber on SetPidState [std_msgs/Empty]
```

Now you can monitor the position from the `GetPos` publisher
```
rostopic echo GetPos
```

### Subscribers

| Topic        | Message Type    | Purpose                                   | Command                                              |
| :----------- |:---------------:| :---------------------------------------- | :--------------------------------------------------- |
 toggle\_motor | std\_msgs/Empty | Toggle Motors on/off, stop PID Controller | `rostopic pub toggle_motor std_msgs/Empty --once`
 home          | std\_msgs/Empty | Set current position as new zero          | `rostopic pub home std_msgs/Empty --once`
 drive\_dist   | std\_msgs/Int32 | Drive <n> encoder clicks                  | `rostopic pub drive_dist std_msgs/Int32 --once -- <n>`
 DriveTo       | std\_msgs/Int32 | Drive until encoder click <n> is reached  | `rostopic pub DriveTo std_msgs/Int16MultiArray --once "$(cat testmsg)`
 DriveDist     | std\_msgs/Int16MultiArray | Drive <n> encoder clicks        | `rostopic pub DriveDist std\_msgs/Int16MultiArray --once "$(cat testmsg)"`

testmsg (in repository folder `t/`) :
```
layout:
  dim:
  -
    label: DriveDist
    size: 7
    stride: 1
  data_offset: 0
data: [0, 1, 2, 3, 4, 5, 6 ]
```

     
