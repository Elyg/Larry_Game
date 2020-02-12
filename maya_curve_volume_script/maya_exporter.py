# To use copy and paste the python code to the python script editor
# A GUI should open
# Create a text file, set it's directory, give a name (if it's the trigger volume)
# Write (overwrited everything to the text) or append (adds to the bottom of the text) to the text file

import maya.cmds as cmds

def executeTriggerScript(directory, startingID, mode):
    #create an empty list
    triggerList = []
    triggerList = cmds.ls(sl=1)
    #check if anything is selected
    if (len(triggerList) == 0):
        print "Nothing is selected"
    else:
        #open the text file and write the trigger volumes to it
        i = startingID
        k = 0
        myFile = open(directory, mode)
        for trigger in triggerList:
           myFile.write(i+str(k)+'\n')
           pos = cmds.exactWorldBoundingBox(trigger)
           for values in pos:
               myFile.write(str(values)+'\n')
           print 'Writingtrigger volume ' +str(i)+': ' + trigger
           k +=1
        myFile.close()
        print 'Done'  
def executeCurveScript(directory, mode):
    # if draw is true it will copy spheres onto the curves to represent the coins
    draw = False
    curveList = []
    #add curves to the list
    curveList = cmds.ls(sl=1)
    myFile = open(directory, mode)
    print curveList[0]
    #check if anything is selected
    if (len(curveList) == 0):
        print "Nothing is selected" 
    else:
        #open the text file and write the CV data  to it
        for curves in curveList:
            numSpans = cmds.getAttr(curves+'.spans')
            degree = cmds.getAttr(curves+'.degree')
            size = (numSpans + degree)
            print 'Number of CVs ' + str(size) + ' in curve' +curves
            for i in range(0, size):
                pos = cmds.pointPosition(curves+'.cv['+str(i)+']', w=True)
                if(draw == True):
                    spheres = cmds.sphere(r=0.2)
                    cmds.move(pos[0],pos[1],pos[2], spheres, absolute=True) 
                for values in pos:
                    myFile.write(str(values)+'\n')
                i+=1
                print 'Writing CV positions: ' + str(i)
    myFile.close()
    print 'Done'  
    
def executeTrigger(*args):
    #GUI variables for trigger volume 
    directory = cmds.textField('DirectoryTrigger', q=True, tx=True)
    ID = cmds.textField('ID',q=True, tx=True)
    mode = "w"
    if (cmds.checkBox('WriteTrigger', q=True, v=True)) == True:
        mode = 'w'
    if (cmds.checkBox('AppendTrigger', q=True, v=True)) == True:
        mode = 'a'
    executeTriggerScript(directory, ID, mode)
    
def executeCurve(*args):
    #GUI variables for CV of curve writing
    directory = cmds.textField('DirectoryCurve', q=True, tx=True)
    mode = "w"
    if (cmds.checkBox('WriteCurve', q=True, v=True)) == True:
        mode = 'w'
    if (cmds.checkBox('AppendCurve', q=True, v=True)) == True:
        mode = 'a'
    executeCurveScript(directory, mode)
   
def triggerExportUI():
    #The GUI
    #define the ID 
    windowID = 'TriggerExport'
    # check if the window exists 
    if cmds.window(windowID, exists=True):
        cmds.deleteUI(windowID)   
    # MAIN LAYOUT   
    mainLayout = cmds.window(windowID, title='Trigger Export', resizeToFitChildren = True, sizeable = False) 
    # main layout
    tab = cmds.frameLayout(label='Export Data Script', collapsable=False, cl=False, mw = 10, mh = 10, width =100, parent=mainLayout)
    cmds.text('The path to the trigger text file:')
    cmds.textField('DirectoryTrigger', tx='e.g. C:/Users/Name/Desktop/testTrigger.txt', ann = 'Path for the script')
    cmds.text('Custom ID:')
    cmds.textField('ID', tx='volume_name_',ann = 'integer ID')
    cmds.checkBox('WriteTrigger',v=True, onc="cmds.checkBox('AppendTrigger',e=True, en=False)",ofc="cmds.checkBox('AppendTrigger',e=True, en=True)")
    cmds.checkBox('AppendTrigger',v=False,en =False, onc="cmds.checkBox('WriteTrigger',e=True, en=False)",ofc="cmds.checkBox('WriteTrigger',e=True, en=True)")
    cmds.button(label='Write Triggers', c=executeTrigger, ann = 'Write Trigger pos') 
    cmds.separator(h=10)
    
    cmds.text('The path to the curve text file:')
    cmds.textField('DirectoryCurve', tx='e.g. C:/Users/Name/Desktop/testCurve.txt', ann = 'Path for the script')
    cmds.checkBox('WriteCurve',v=True, onc="cmds.checkBox('AppendCurve',e=True, en=False)",ofc="cmds.checkBox('AppendCurve',e=True, en=True)")
    cmds.checkBox('AppendCurve',v=False,en =False, onc="cmds.checkBox('WriteCurve',e=True, en=False)",ofc="cmds.checkBox('WriteCurve',e=True, en=True)")
    cmds.text('Write (over writes everything in file)\n Append ( adds to file (dont forget to change the starting ID )') 
    cmds.button(label='Write Curves', c=executeCurve, ann = 'Write CVs')
    cmds.separator(h=10)
    cmds.showWindow(windowID)  

triggerExportUI()
