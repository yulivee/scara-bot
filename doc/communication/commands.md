<a name="comm_def" href="#"></a>
### Command Definition

|Command Name|Command Code|Command Data|Requested Data|Slave Function|
|:-:|:-:|:-:|:-:|:--|
|ping_slave     |0  |`ping`     |`echo`         |Echo `data` back to master  |
|home           |1  |/          |/              |Set current position to zero |
|set_pid_state  |5  |`state`    |/              |Set PID to ON or OFF |
|get_position   |6  |/          |`motor_count`  |Send current position to master  |
|get_target   |7  |/          |`target_position`  |Send target position to master  |
|get_slave_num|8|/|`slave_number`|Debug fuction|
|drive_dist     |10 |`distance` |/              |Change target position by `distance` |
|drive_dist_max |11 |`distance` |/              |Move joint by `distance` from current position |
|drive_to       |12 |`position` |/              |Move joint to position |

Expected Values:  
- `slave_number`: 1-7
- `state`: 0 or 1 (interpreted as bool)
- `distance`: -32,768 to 32,767 in clicks (2 byte int)
- `position`: -32,768 to 32,767 clicks (2 byte int)
- `motor_count`: -32,768 to 32,767 clicks (2 byte int)
- `target_position`: -32,768 to 32,767 clicks (2 byte int)
