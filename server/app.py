import uuid;
from flask import Flask, render_template,request,jsonify
from flask_sock import Sock
from src.connections.robot_connection import RobotConnection
from src.connections.human_connection import HumanConnection
from src.services.users_manager import UsersManager
app = Flask(__name__)
app.config['SOCK_SERVER_OPTIONS'] = {'ping_interval': 1}
sock = Sock(app)

@sock.route('/connect/<connection_type>')
def socket_connection(ws, connection_type):
    if connection_type == 'person':
        conection = HumanConnection(ws)
    elif connection_type == 'robot':
        conection = RobotConnection(ws)
    else:
        print(f"Invalid connection type provided: {connection_type}.")
        return
    conection.start_loop_conection()

@app.route('/robot/<robotid>')
def getRobot(robotid):
    return jsonify(UsersManager().get_user(uuid.UUID(robotid)))

@app.route("/robots")
def getRobots():
    robots_uid_list = [robot.uid for robot in UsersManager().get_all_connected_robots()]
    return jsonify(robots_uid_list)

@app.route('/joystick')
@app.route('/joystick/<robotId>')
def joystick(robotId=None):
    return render_template('joystick.html',robotId=robotId)

@app.route('/')
def homePage():
    return render_template('index.html')
