<a name="comm_def" href="#"></a>
### Command Definition

|Command Name|Command Code|Command Data|Requested Data|Slave Function|
|:-:|:-:|:-:|:-:|:--|
|ping           |0  |`data`     |`data` |Echo `data` back to master  |
|home           |1  |0       |0              |Set current position to zero |
|set_pid_state  |5  |`state`    |0              |Set PID to ON or OFF |
|get_position   |6  |0       |`motor_count`  |/  |
|drive_dist     |10 |`distance` |0              |Change target position by `distance` |
|drive_dist_max |11 |`distance` |0              |Move joint by `distance` from current position |
|drive_to       |12 |`position` |0              |Move joint to position |

Expected Values:  
- `slave_number`: 1-7
- `state`: 0 or 1 (interpreted as bool)
- `distance`: -32,768 to 32,767 in clicks (2 byte int)
- `position`: -32,768 to 32,767 clicks (2 byte int)
- `motor_count`: -32,768 to 32,767 clicks (2 byte int)
