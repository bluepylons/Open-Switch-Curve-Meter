import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

switch_name = "Kailh Box Navy" #switch_name is currently not used
input_file = r"C:\Users\*User*\Documents\GitHub\Open-Switch-Curve-Meter\Processing-Python-script\Cherry-MX-Purple-RGB.csv"
output_file = r"C:\Users\*User*\Documents\GitHub\Open-Switch-Curve-Meter\Processing-Python-script\Cherry-MX-Purple-RGB-processed.csv"

graph=False #Graphing still needs to be implemented
graph_output_file = r"Wherever.png" #Graphing still needs to be implemented

#parameters used to find the start and end (adjust if they don't work well)
start_step = 0.3 #This is the change in consecutive measurements that indicates the start point for the force curve 
end_step = start_step # This is the change in consequtive measurements that  indicates the  end point - 

#property of your force curve meter
force_curve_meter_step_increment = 0.005 #the increments your force curve meter steps in (in mm)

#properties of the actuation and release arrows
arrow_length = 10 #length for the actuation and release point arrows
arrow_offset = 3 #How far the arrows should be from the actual force curve line, in gf 

#start conditions used to find the start and end 
started = False 
index_start = 0 
start_point = 0
zero_average = 0
new_x_value = 0

ended = False
index_end = 0
end_point = 0

#start conditions of the variables used to find the actuation point 
actuated = False
index_actuation = 0 
actuation_point = 0
actuation_force = 0

released = False
index_release = 0 
release_point = 0
release_force = 0

#Part 0 - import data
data = pd.read_csv(input_file)

#Part 1 - Tare the data, and prepare the downstroke 

#Part 1a - find the start point of the downstroke - look at the change in data, and when the change is greater than start_step, we've found the start point

while started==False:
    index_start += 1 #when the While loop exits this variable is used in the rest of the program 
    if(data["Raw Force (gf)"][index_start]-data["Raw Force (gf)"][index_start-1]>start_step):
        started=True #exit the while loop
    elif(index_start > 3000): #Error - no start point found - prevent infinite loop 
        print("Error - unable to find start point - consider adjusting your start_step, or check your input file.")
        break

#Part 1b - tare the data - find the average of the values before the start point, and subtract that from the raw data to tare them
zero_average = np.average(data["Raw Force (gf)"][0:index_start-1]) #-1, as sometimes you have small spikes at the very beginning that are less than start_step - we don't want that counted towards the average
data["Force - Tared (gf)"] =  data["Raw Force (gf)"] - zero_average

#Part 1c - create the corrected downstroke
#Part 1c1 - find the length of the downstroke (travel)
peak_value = data["Force - Tared (gf)"].max() #this is the bottom out point 
index_peak = data["Force - Tared (gf)"].idxmax()

travel = (index_peak - index_start)*force_curve_meter_step_increment


#Part1c2 - create the corrected downstroke X axis which starts from 0
data["Corrected downstroke X (mm)"] = pd.Series(np.arange(0,travel+force_curve_meter_step_increment,force_curve_meter_step_increment))

#Part 1c3 - create the corrected downstroke force readings:
downstroke_force = data["Force - Tared (gf)"][0:index_peak+1]
data["Corrected downstroke force (gf)"] = downstroke_force.shift(-1*index_start) #remove the forces before the start point, and shift everything up

#Part 1c4 - create the corrected downstroke actuation status:
downstroke_actuation = data["Actuated?"][0:index_peak+1]
data["Corrected downstroke actuated?"] = downstroke_actuation.shift(-1*index_start) #remove the values before the start point, and shift everything up

#Part 2 - Prepare the upstsroke

#Part 2a - create the corrected uptroke X axis which starts from the travel measurement and goes down
data["Corrected upstroke X (mm)"] = pd.Series(np.arange(travel,-1*force_curve_meter_step_increment,-1*force_curve_meter_step_increment)) #range does not include the second argument

#Part 2b - Find the endpoint of the measurement #start from the end- occasionally the load cell drifts slightly over the course of the measurement, so the endpoint may not return exactly to zero
index_end = len(data)-1 

while ended==False:
    index_end -= 1 #when the While loop exits this variable is used in the rest of the program 
    if(data["Force - Tared (gf)"][index_end]-data["Force - Tared (gf)"][index_end+1]>end_step):
        ended=True #exit the while loop
    elif(index_end <= 0): #Error - no endpoint found - prevent infinite loop 
        print("Error - unable to find end point - consider adjusting your end_step")
        break
        
#Part 2c - create the corrected upstroke force 
upstroke_force = data["Force - Tared (gf)"][0:index_end+1]
data["Corrected upstroke force (gf)"] = upstroke_force.shift(-1*(index_peak+1))

#Part 2d - create the corrected upstroke actuation status:
upstroke_actuation = data["Actuated?"][0:index_end+1]
data["Corrected upstroke actuated?"] = upstroke_actuation.shift(-1*(index_peak+1))

#Part 2e - match length , by trimming from the top (this tends to correct backlash well for some reason)
if(data["Corrected upstroke force (gf)"].count()>data["Corrected upstroke X (mm)"].count()): #if the upstroke is longer than the downstroke -f trim off the top
    data["Corrected upstroke force (gf)"] = data["Corrected upstroke force (gf)"].shift(-1*(data["Corrected upstroke force (gf)"].count()-data["Corrected upstroke X (mm)"].count()))
    data["Corrected upstroke actuated?"] = data["Corrected upstroke actuated?"].shift(-1*(data["Corrected upstroke actuated?"].count()-data["Corrected upstroke X (mm)"].count()))
    
elif(data["Corrected upstroke force (gf)"].count()<data["Corrected upstroke X (mm)"].count()): #if the upstroke is shorter than the downstroke - add zeroes to the end 
    #still need to write this code to fill in zeroes
    print("test")

#Part 3 - find and report the actuation and release points

#Part 3a - find the actuation point - where "Actuated?" changes from 0 to 1 - by iterating the column until it is found
while actuated==False:
    index_actuation += 1 #when the While loop exits this variable is used in the rest of the program 
    if(data["Corrected downstroke actuated?"][index_actuation]>0.5 and data["Corrected downstroke actuated?"][index_actuation-1]<0.5): #these should be 1's and 0's, but int can't handle NaN
        actuated=True #exit the while loop
        actuation_point = data["Corrected downstroke X (mm)"][index_actuation]
        actuation_force = data["Corrected downstroke force (gf)"][index_actuation]
    elif(index_actuation > 6000): #Error - no start point found - prevent infinite loop 
        print("Error - unable to find actuation point")
        break
        
#Part 3b - find the release point - where "Actuated?" changes from 1 to 0 - by iterating the column until it is found
while released==False:
    index_release += 1 #when the While loop exits this variable is used in the rest of the program
    if(data["Corrected upstroke actuated?"][index_release]<0.5 and data["Corrected upstroke actuated?"][index_release-1]>0.5): #these should be 1's and 0's, but int can't handle NaN
        released=True #exit the while loop
        release_point = data["Corrected upstroke X (mm)"][index_release]
        release_force = data["Corrected upstroke force (gf)"][index_release]
    elif(index_actuation > 6000): #Error - no start point found - prevent infinite loop 
        print("Error - unable to find actuation point")
        break
        
#Part 3c - write the actuation points
data["Actuation points"] = np.nan
data.at[0,"Actuation points"]=actuation_point
data.at[1,"Actuation points"]=actuation_point

data["Actuation arrow points"] = np.nan
data.at[0,"Actuation arrow points"]=actuation_force + arrow_offset
data.at[1,"Actuation arrow points"]=actuation_force + arrow_offset + arrow_length

#Part 3d - write the release points
data["Release points"] = np.nan
data.at[0,"Release points"]=release_point
data.at[1,"Release points"]=release_point

data["Release arrow points"] = np.nan
data.at[0,"Release arrow points"]=release_force - arrow_offset
data.at[1,"Release arrow points"]=release_force - arrow_offset - arrow_length

#Part 4 - write output file
data = data.round(3) #clean up excessive decimal places 
data.to_csv(output_file)

#Part 5 - Report findings:
print("Index start: ",index_start)
print("Index peak: ",index_peak)
print("Index end: ",index_end)
print("Travel:", travel, "mm")
print("Actuation point: ",actuation_point,"mm")
print("Actuation force: ",actuation_force,"gf")
print("Release point: ",release_point,"mm")
print("Release_force: ",release_force,"gf")

#Part 6 - Graph (not yet implemented)

if(graph):
    data.plot(x=data["Corrected downstroke X (mm)"], y=data["Corrected downstroke force (gf)"])