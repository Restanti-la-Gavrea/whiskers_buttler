import sys;
from flask import Flask, render_template,request,jsonify
from flask_sock import Sock

app = Flask(__name__)
sock = Sock(app)
list = []


@sock.route('/echo')
def echo(ws):
    # list.append(ws)
    # if len(list ) > 1:
    #     list[0].send("hello")
    while True:
        data = ws.receive()
        ws.send(data)

@app.route("/get_my_ip", methods=["GET"])
def get_my_ip():
    return jsonify({'ip': request.remote_addr}), 200

@app.route('/')
@app.route('/<name>')
def hello(name=None):
    return render_template('hello.html', name=name)
