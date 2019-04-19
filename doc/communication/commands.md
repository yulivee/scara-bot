<a name="comm_def" href="#"></a>
### Command definition for slaves

|Command Name|Code|Recieved Data|Sent Data|Slave Function|
|:-:|:-:|:-:|:-:|:--|
|ping_slave     |0  |`ping`     |`echo`           |Echo `data` back to master  |
|home           |1  |/          |/                |Set current position to zero |
|set_pid_state  |2  |`state`    |/                |Set PID to ON or OFF |
|get_position   |6  |/          |`motor_count`    |Send current position to master  |
|get_target     |7  |/          |`target_position`|Send target position to master  |
|get_slave_num  |8  |/          |`slave_number`   |Debug fuction|
|drive_dist     |10 |`distance` |/                |Change target position by `distance` |
|drive_dist_max |11 |`distance` |/                |Move joint by `distance` from current position |
|drive_to       |12 |`position` |/                |Move joint to position |
|set_speed      |15 |`speed`    |/                |Set speed in percent|
|set_zone       |16 |`zone`     |/                |set fly-by distance to target|
|check_target_reached|20  |/    |`target_reached` |Tell master if target position is reached (within zone)|


### Expected values
|Variable|Type on Slave|Type on Master|Expected Values on Bus|
|:-:|:-:|:-:|:-:|
|`slave_number` |int          |int array| 1-7|
|`state`        |int          |bool     | 0 or 1 (interpreted as bool)|
|`distance`     |int          |int array| -32,768 to 32,767 in clicks (2 byte int)|
|`position`     |int          |int array| -32,768 to 32,767 clicks (2 byte int)|
|`motor_count`  |volatile int |int array| -32,768 to 32,767 clicks (2 byte int)|
|`target_position`|int        |int array| -32,768 to 32,767 clicks (2 byte int)|
|`speed`        |int          |int      | 0-100 (interpreted as %)|
|`zone`         |int          |int      | 0-32,767 in clicks|
|`target_reached`|bool        |int      |  0 or 1 (interpreted as bool)|


### Error codes
`error_code`:
-  no_error = 0,
-  command_offs =100,
-  e_wrong_slave = 910,
-  e_ping_bad_echo = 920,
-  e_unknown_command = 930,
-  e_bad_data = 940,
-  default_value = 990
