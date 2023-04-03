import serial
import serial.tools.list_ports as list_port
import time as timeLib
import csv

from dash import Dash, dcc, html, Input, Output
import plotly.graph_objects as go
import plotly.subplots as make_subplots


# class for each set of board numbers
class IndivBoard:

    # creates an instance of itself
    def __init__(self, boardNum):
        self.boardNum = boardNum
        self.sensors = []
        # create 4 sensors per board, some might not be used
        for i in range(1,5):
            a = IndivSensor(str(i))
            self.sensors.append(a)

    # adds data by calling sensors class update
    def addData(self, senType, data):
        for sensor in self.sensors:
            if senType == sensor.senType:
                sensor.updateSelf(data)

class IndivSensor:

    # creates an instance of itself
    def __init__(self, senType):
        # default values for sensor readings is 0 and current time (of sensor init starting)
        self.data=[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
        x = timeLib.time()
        self.time = [x,x,x,x,x,x,x,x,x,x,x,x,x,x,x]
        # stores the type of sensor
        self.senType = senType
        # if the sensor reacives no update, it will not be graphed. This is how we know
        self.updated = False

    def updateSelf(self, data):
        # when updated, it removes the first element then adds to the end. Its a queue but with python code.
        self.data.pop(0)
        self.data.append(data)
        self.time.pop(0)
        self.time.append(timeLib.time())
        # stated that this sensor needs to now be graphed
        self.updated = True

    def printData(self):
        # DEBBUG CODE, prints out the sensor and the last data received
        print("---  " + self.senType + "   ---   " + str(self.data[-1]) + " - " + str(self.time[-1]))


# --------------------------------------------------------------------------------------------------------
# code runs at start


# init serial reading of arduino through the serial ports
serialInst = serial.Serial()
serialInst.baudrate = 9600
serialInst.port = 'COM4'
serialInst.open()

# variable used for later
boards = []

# clears out the first bit of serial (it could be cut off)
while not serialInst.in_waiting:
    timeLib.sleep(0.001)
package = serialInst.readline()

# makes a temp graph object to give to the webserver
graphs = make_subplots.make_subplots(rows=4, cols=1)

#
with open('dataOut.csv', 'w', newline='') as file:
    writer = csv.writer(file)

# --------------------------------------------------------------------------------------------------------
# init the website
app = Dash(__name__, update_title=None)

# the website
app.layout = html.Div(children=[
    dcc.Graph(
        id='graph1',
        figure=graphs
    ),
    dcc.Interval(
        id="sensor_update",
        interval=1000,
        n_intervals=0
    )
])


# when ever the website calles on the code (at 1 sec intervals) it then reads serial and updates the graphs
@app.callback(Output('graph1', 'figure'),
              Input('sensor_update', 'n_intervals'))
def sensorLoop(n):
    # do nothing if no new data is received
    if serialInst.in_waiting:

        good = True                         # temp variable for indicating a good read
        package = serialInst.readline()     # reads the serial
        line = package.decode('utf')        # decodes to normal text

        # attempts to get data (as an int) and the ID + sensor type
        try:
            numData = int(line[4:])
        except:
            numData = 0
            good = False
            print("Data is bad")

        try:
            ID = line[:2]
            sensor = line[2]
        except:
            ID = '00'
            sensor = '0'
            good = False
            print("Data is bad")

        # if an error occurs skip the rest of the code
        if good:
            # temp var
            found = False
            for board in boards:
                if ID == board.boardNum:
                    # goes through the board, find the one that has been called and adds data
                    board.addData(sensor, numData)
                    found = True

            # if the board is not found a new element is added to the list and data is added to the board
            if not found:
                boards.append(IndivBoard(ID))
                boards[-1].addData(sensor, numData)

            # adds data to cvs
            with open('dataOut.csv', 'a') as file:
                writer = csv.writer(file)
                writer.writerow([timeLib.time(), ID, sensor, numData])

        # makes a graph subplot which is actual space for 4 graphs
        graphs = make_subplots.make_subplots(rows=4, cols=1)

        # goes through each board and adds a new line for every updated sensor
        for board in boards:
            for j in range(1,5):
                if board.sensors[j-1].updated:
                    # makes the graph read able
                    name = ''
                    if j == 1:
                        name += 'Photo'
                    elif j == 2:
                        name += 'Pot'
                    elif j == 3:
                        name += 'Temp'
                    elif j == 4:
                        name += 'Humid'
                    name += " -" + board.boardNum
                    # adds the traces
                    graphs.add_trace(row=j,col=1,
                                     trace=go.Scatter(x=board.sensors[j-1].time,
                                                      y=board.sensors[j-1].data,
                                                      name=name ))
        # makes the graph of the right size
        graphs.update_layout(height=1200, width=1800)

        # returns the graph to the app callback to be printed in the website
        return graphs


# the website
app.run_server(debug=False)
