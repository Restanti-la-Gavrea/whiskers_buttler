## How to run server locally
 - flask run --host=0.0.0.0 
## How to deply server
 - fly deploy


| Value, Name | Parameter, Size(bytes) | Meaning |
|:----:|:----:|:------------|
| `00`, Register | {`UID`} / `NULL`, 36/0 | This command is utilized by the user to either register or authenticate. Sending NULL initiates a new registration process to obtain a unique ID. To authenticate with an existing ID, the user must provide that specific UID. The server also uses this command to assign or confirm a specific UID to a user. | 
| `01`, Link | {`UID`} / `NULL`, 36/0 | This command is used by a person attempting to connect to a robot by specifying the UID. The server responds with the same `UID` to confirm a successful connection if the `UID` exists, or with `NULL` if the connection attempt does not succeed. |
| `02` | {`UID`}, 36 | This Command is used by a person who wants to begin a match with another person. The command is parse by the serve which sends `NULL` back in case of rejection. | 
| `03` | {`time`}, 1   | This command is send by the server to users to notify them that a match starts in `t` secconds. if `time` is equal to `0` then the match starts. If `time` is equal to `FF` then the match stops. | 
| `04` | {`True`/`False`}, 1     | This command is sent by the person or server to a robot if they want or do not want to receive video streaming. | 
| `05` | {`frame`} variable size     | This message contains a JPEG frame send by a robot. The frame is parsed by the server. |
| `06` | {`MotorLeft`, `MotorRight`}, 2 | This coomand is used by a person or the server to set the motor power (left and right) of a robot, or by the robot to notify the server by its current motor power. MotorLeft and Right are variable of 1 byte. |

### ESP32-CAM
    - datasheet : https://media.digikey.com/pdf/Data%20Sheets/DFRobot%20PDFs/DFR0602_Web.pdf

### Cumparaturi 1

https://www.emag.ro/convertor-descendent-hw-613-4-5v-24v-la-0-8v-21v-3a-multicolor-5904162805419/pd/DXBZNLMBM/?X-Search-Id=7a42ea977063dd6d1c12&X-Product-Id=100502069&X-Search-Page=1&X-Search-Position=33&X-Section=search&X-MB=0&X-Search-Action=view#specification-section



