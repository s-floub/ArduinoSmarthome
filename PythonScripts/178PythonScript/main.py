import serial
import datetime
import time as timeLib
from pandas import DataFrame
import openpyxl

import plotly.graph_objects as go
import plotly.express as px
import plotly.subplots as make_subplots


fileLocation = "C:\\Users\\imraa\\OneDrive\\Documents\\GitHub\\ArduinoSmarthome\\PythonScripts\\178PythonScript\\dataOut.xlsx"

class IndivSensor:

    def __init__(self):
        self.data=[0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
        x = timeLib.time()
        self.time = [x,x,x,x,x,x,x,x,x,x]
        self.number = ""
        self.type = '0'
        self.df = DataFrame(data=self.data, index=self.time)

    def printInfo(self):
        print(" ----------------- " + self.number + self.type + '---------------')
        print(self.df)

    def updataSelf(self):
        self.time.pop(0)
        self.time.append(timeLib.time())
        self.df = DataFrame(data=self.data, index=self.time)

def main():
    serialInst = serial.Serial()
    serialInst.baudrate = 9600
    serialInst.port = 'COM4'
    serialInst.open()

    var = 0

    # dataOut = openpyxl.load_workbook(fileLocation)

    data = []
    while not serialInst.in_waiting:
        timeLib.sleep(0.001)

    package = serialInst.readline()

    while True:
        if serialInst.in_waiting:
            package = serialInst.readline()
            line = package.decode('utf')
            time = str(datetime.datetime.now().strftime('%H:%M:%S.%f'))

            ID = line[:3]

            try:
                numData = int(line[4:])
            except:
                numData = 0
                print("Data is bad")

#            found = False
#            for i in dataOut.worksheets:
#                if ID == 1:
#                    found = True
#            if not found:
#                dataOut.create_sheet(ID)
#                dataOut.save(fileLocation)

            found = False
            for i in data:
                if ID[:2] == i.number and ID[2] == i.type:
                    i.data.pop(0)
                    i.data.append(numData)
                    i.number = line[:2]
                    i.type = line[2]
                    i.updataSelf()
                    #i.printInfo()
                    found = True
            if not found:
                data.append(IndivSensor())
                data[-1].data.pop(0)
                data[-1].data.append(numData)
                data[-1].number = line[:2]
                data[-1].type = line[2]
                data[-1].updataSelf()
                #data[-1].printInfo()

            fig = make_subplots.make_subplots(rows=1, cols=1)
            for i in data:
                fig.add_trace(go.Scatter(x=i.time, y=i.data))
                var= var+1
            fig.show()



main()
