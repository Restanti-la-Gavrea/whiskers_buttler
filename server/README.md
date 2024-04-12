## How to run server locally
 - flask run --host=0.0.0.0 
## How to deply server
 - fly deploy


| Value | Name         | Parameter           | Parameter Size(bytes)      | Meaning                                                                                                                                                                         |
|-------|--------------|---------------------|------------------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `00`  | Register     | `{UID}` / `NULL`    | 36/0             | This command is utilized by the user to either register or authenticate. Sending NULL initiates a new registration process to obtain a unique ID. To authenticate with an existing ID, the user must provide that specific UID. The server also uses this command to assign or confirm a specific UID to a user. |
| `01`  | Link         | `{UID}` / `NULL`    | 36/0             | This command is used by a person attempting to connect to a robot by specifying the UID. The server responds with the same `UID` to confirm a successful connection if the UID exists, or with `NULL` if the connection attempt does not succeed. This can also be sent by the server to announce to the person that the connection to the robot is lost. |
| `02`  | Connection  | `{True/False}`      | 1    | This command is used by the server to indicate that the connection to the robot is lost or established.                                                                                                                                      |
| `04`  | Video Stream | `{True/False}`      | 1                | This command is sent by the person or server to a robot if they want or do not want to receive video streaming.                                                                                                                             |
| `05`  | Frame        | `{frame}`           | Variable    | This message contains a JPEG frame sent by a robot. The frame is parsed by the server.                                                                                                                                                     |
| `06`  | Motor Power  | `{MotorLeft, MotorRight}` | 2            | This command is used by a person or the server to set the motor power (left and right) of a robot, or by the robot to notify the server of its current motor power. MotorLeft and MotorRight are variables of 1 byte each.                   |
| `07`  | Distance Data  | `{data0,..., data12}` | 13            | This command is used by the robot to send the dstances detected in many direction from 0 to 180 degree from 15 to 15 degreee. |
| `08`  | Ring Edge Data  | `{data}` | 1            | This command transmits the edge detection status from up to five infrared sensors, with each sensor's status encoded into a single bit of the data byte. A bit value of 1 indicates an edge is detected by the corresponding sensor, while 0 means no detection. The least significant bit represents the first sensor, progressing to the fifth sensor at the fifth least significant bit. This compact format allows for efficient communication of the robot's proximity to ring edges.               |


### ESP32-CAM
    - datasheet : https://media.digikey.com/pdf/Data%20Sheets/DFRobot%20PDFs/DFR0602_Web.pdf

### Cumparaturi 1

https://www.emag.ro/convertor-descendent-hw-613-4-5v-24v-la-0-8v-21v-3a-multicolor-5904162805419/pd/DXBZNLMBM/?X-Search-Id=7a42ea977063dd6d1c12&X-Product-Id=100502069&X-Search-Page=1&X-Search-Position=33&X-Section=search&X-MB=0&X-Search-Action=view#specification-section



