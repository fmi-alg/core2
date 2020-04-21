import sys
import fileinput
import os
import math

# change these paths accordingly
input_path = r'E:\robotics\triangle\inputs'
file_name = 'bugtrap.txt'
output_path = r'E:\OOPSMP-1.1.1\build\bin\Release'

def main():
    print "Reading file " + os.path.join(input_path, file_name)
    pts = []
    polys = []
    fn = open(os.path.join(input_path, file_name))
    #get rid of emply lines and comments, as well as comment in same line
    content = [x[:x.find('#')].strip().split() for x in fn if x.strip().split() and x.strip().split()[0] != '#']
    #print content
    
    ptsCt = int(content[0][0])
    
    #search for the line contain polygon count
    for i, s in enumerate(content[1:]):
        if len(s) == 1:
            polyCt = int(s[0])
            break
    #print polyCt
    #print i
    
    #lines in content[1:i+1] contains pts coordinates
    pts_it = iter([float(c) for sublist in content[1:i+1] for c in sublist])
    
    #http://stackoverflow.com/questions/2233204/how-does-zipitersn-work-in-python
    pts = zip(*[pts_it]*2)
    #print pts
    assert(len(pts) == ptsCt)
    
    #lines in content[i+2:] contains polygon information
    poly_list = content[i+2:]
    polys = [poly_list[0]]
    for it in poly_list[1:]:
        #if end with \, next line is a continue of previous line
        if polys[-1][-1] == '\\':
            polys[-1] = polys[-1][:-1]
            polys[-1].extend(it)
        elif polys[-1][-1].endswith('\\'):
            polys[-1][-1] = polys[-1][-1][:-1]
            polys[-1].extend(it)
        else:
            polys.append(it)
    polys = [map(int, sublist)for sublist in polys]
    #print polys
    assert(len(polys) == polyCt)
	    
    obstacles = []
    for p in polys:
        cords = []
        for i in p[:-1]:
            cords.append(' '.join(map(str, [coord / 512.0 - 0.5 for coord in pts[i-1]])) + '\n             ')
        cords[-1] = cords[-1].rstrip()
        obstacles.append( addStaticPart.format(len(cords), ''.join(cords)) )
    
    r = raw_input("enter Robo size:(R0, theta1, theta2)\n")
    triRobo = map(float, r.strip().split(','))
    theta = [None]*3
    theta[0] = 0.0
    theta[1:3] = [t * 3.14159 for t in triRobo[1:3]]
    print theta
    roboVerts = []
    for t in theta:
        roboVerts.append( (triRobo[0]  / 512.0 * math.cos(t), triRobo[0] / 512.0 * math.sin(t)) )
    print roboVerts
    robo = addMovingPart.format(*[' '.join(map(str, roboVerts[j])) for j in xrange(3)])
    print robo
    
    r = raw_input("enter alpha: (x, y, angle)\n")
    alpha = map(float, r.strip().split(','))
    alpha[:2] = [coord / 512.0 - 0.5 for coord in alpha[:2]]
    alpha[2] = (alpha[2] % 2 - 1) * 3.14159
    r = raw_input("enter beta: (x, y, angle)\n")
    beta =  map(float, r.strip().split(','))
    beta[:2] = [coord / 512.0 - 0.5 for coord in beta[:2]]
    beta[2] = (beta[2] % 2 - 1) * 3.14159
    print alpha, beta
    query = setQueries.format(' '.join(map(str, alpha)), ' '.join(map(str, beta)))
    #print query
	
    output_file = open(os.path.join(output_path, file_name), 'w')
    print >> output_file, xml.format(query, robo, ''.join(obstacles))
    
setQueries = \
'''<call fn="setQueries"><arg type="pointer">
      <factory instance="Queries">
<call fn="addQueries">
 <arg type="int">1</arg>
 <arg type="int">3</arg>
 <arg type="double_array">
   {0}
   {1}
 </arg>
</call></factory>'''

addMovingPart = \
'''<call fn="addMovingPart"><arg type="pointer">
 <factory instance="Part2D">
   <comment>
   Geometry for a rigid-body 2D robot. A call to centerAtZero function
   centers the geometry at zero, which is necessary for the
   SE2Topology and SE2StateSpace to work correctly. This call must be
   after setting all the vertices.    
   </comment>

         <call fn="setVertices">
          <arg type="int">3</arg>
          <arg type="double_array">
             {0} 
             {1} 
             {2} 
          </arg>
        </call>
 
	<call fn="centerAtZero"></call>
	
</factory>

</arg></call>'''

#{0} is # of vertices, {1} coordinates
addStaticPart = \
'''    <call fn="addStaticPart"><arg type="pointer">
      <factory instance="Part2D">
        <call fn="setVertices">
          <arg type="int">{0}</arg>
          <arg type="double_array">
             {1}
          </arg>
        </call>
       </factory>
    </arg></call>

'''
    
#{0} for setQueries, {1} for addMovingPart, {2} for addStaticPart
xml = \
'''<factory instance="GGLUTEventManager">
<call fn="setProgram">
<arg type="pointer">

<factory instance="GMotionPlannerProgram">    

   <comment>
   This program reads queries from the file
   Core/Query/Queries.xml and then runs the selected
   motion planner interactively. 
   </comment>

   <call fn="setGViewerController"><arg type="pointer">
       <factory instance="GViewerController">

    <call fn="setGViewer"><arg type="pointer">
      <factory instance="GViewer">

  <call fn="setViewportWidth">         <arg type="double">600</arg></call>
  <call fn="setViewportHeight">        <arg type="double">600</arg></call>

  <call fn="setProjectionTypeByName">  <arg type="string">Perspective</arg></call>
  <call fn="setProjectionLeftPlane">   <arg type="double">-0.551</arg></call>
  <call fn="setProjectionRightPlane">  <arg type="double"> 0.551</arg></call>
  <call fn="setProjectionBottomPlane"> <arg type="double">-0.591</arg></call>
  <call fn="setProjectionTopPlane">    <arg type="double"> 0.551</arg></call>
  <call fn="setProjectionNearPlane">   <arg type="double"> 0.05</arg></call>
  <call fn="setProjectionFarPlane">    <arg type="double">5.000</arg></call>
  <call fn="setProjectionAspectRatio"> <arg type="double">1.33333333333333</arg></call>
  <call fn="setProjectionViewingAngleInDegrees"> <arg type="double">60</arg></call>

</factory>

    </arg></call>

</factory>

   </arg></call>
 
   <call fn="setGIllumination"><arg type="pointer">
      <factory instance="GIllumination">

    <call fn="setAmbient"><arg type="float_array">0.2 0.2 0.2 1.0</arg></call>

    <call fn="addGLight"><arg type="pointer">
       <factory instance="GLight">
          <call fn="setAmbient">    <arg type="float_array">1.0 1.0 1.0 1.0</arg></call>
          <call fn="setDiffuse">    <arg type="float_array">1.0 1.0 1.0 1.0</arg></call>
	  <call fn="setSpecular">   <arg type="float_array">0.8 0.8 0.8 1.0</arg></call>
	  <call fn="setAttenuation"><arg type="float_array">0.0 0.03 0.003</arg></call>
	  <call fn="setPosition">   <arg type="float_array">14.0 14.0 15.2 0.0</arg></call>
       </factory>
    </arg></call>


</factory>

   </arg></call>
  
   <call fn="setFileNames"><arg type="pointer">
      <factory instance="FileNames">

   <comment>
   Catalog of input/output files used by different OOPSMP programs.
                                                                    
   Each file name/path is associated with a keyword that is
   used by OOPSMP programs to retrieve the appropriate file
   name/path. Each OOPSMP program may use zero or more files.
   </comment>

   <call fn="addFileName">
     <arg type="string">MotionPlanningDataFile </arg>
     <arg type="string">MotionPlanningDataFile.txt</arg>
   </call>

   <call fn="addFileName">
     <arg type="string">MotionPlanningSolutionsFile</arg>
     <arg type="string">MotionPlanningSolutionsFile.txt</arg>
   </call>

   <call fn="addFileName">
     <arg type="string">MotionPlanningQueriesFile</arg>
     <arg type="string">Core/Query/Queries.xml</arg>
   </call>

   <call fn="addFileName">
     <arg type="string">GraphicsFile</arg>
     <arg type="string">graphics.ps</arg>
   </call>


</factory>

   </arg></call>

   {0}

   </arg></call>

   <call fn="setCoreRobotData"><arg type="pointer">
      <factory instance="CoreRobotData">

    <comment>
    Setting up core components for single-robot problems.

    User sets up workspace, collision detector, robot state space,
    robot valid state sampler, robot path generator, and local
    planner. 
 
    The workspace should have at least one moving part.
    The geometry of the robot is assumed to be that of
    the first moving part in the workspace.
    </comment>  

   <call fn="setWorkspace"><arg type="pointer">
      <factory instance="Workspace2D">

  <comment>
Set the number of moving parts (robots).
Each call adds one moving part.
</comment>


{1}

    <call fn="addStaticPart"><arg type="pointer">
      <factory instance="Part2D">
        <call fn="setVertices">
          <arg type="int">4</arg>
          <arg type="double_array">
             -0.550000 -0.550000
             0.550000 -0.550000
             0.550000 -0.500000
             -0.550000 -0.500000
          </arg>
        </call>
       </factory>
    </arg></call>

    <call fn="addStaticPart"><arg type="pointer">
      <factory instance="Part2D">
        <call fn="setVertices">
          <arg type="int">4</arg>
          <arg type="double_array">
             -0.550000 0.500000
             0.550000 0.500000
             0.550000 0.550000
             -0.550000 0.550000
          </arg>
        </call>
       </factory>
    </arg></call>

    <call fn="addStaticPart"><arg type="pointer">
      <factory instance="Part2D">
        <call fn="setVertices">
          <arg type="int">4</arg>
          <arg type="double_array">
             -0.550000 -0.500000
             -0.500000 -0.500000
             -0.500000 0.500000
             -0.550000 0.500000
          </arg>
        </call>
       </factory>
    </arg></call>

    <call fn="addStaticPart"><arg type="pointer">
      <factory instance="Part2D">
        <call fn="setVertices">
          <arg type="int">4</arg>
          <arg type="double_array">
             0.500000 -0.500000
             0.550000 -0.500000
             0.550000 0.500000
             0.500000 0.500000
          </arg>
        </call>
       </factory>
    </arg></call>

{2}


</factory>



   </arg></call>

   <call fn="setCollisionDetector"><arg type="pointer">
      <factory instance="PQPCollisionDetector2D">
</factory>

   
   </arg></call>

   <call fn="setStateSpace"><arg type="pointer">
      <factory instance="SE2StateSpace">

   <call fn="setDistanceStateCoeffPosX"><arg type="double">1.0</arg></call>
   <call fn="setDistanceStateCoeffPosY"><arg type="double">1.0</arg></call>
   <call fn="setDistanceStateCoeffOrientation"><arg type="double">0.2</arg></call>

</factory>
   </arg></call>

   <call fn="setValidStateSampler"><arg type="pointer">
      <factory instance="UniformValidStateSampler">
</factory>
   </arg></call>

   <call fn="setPathGenerator"><arg type="pointer">
      <factory instance="SE2DecoupledGeodesicPathGenerator">
</factory>
   </arg></call>
  
   <call fn="setLocalPlanner"><arg type="pointer">
      <factory instance="SubdivisionLocalPlanner">

   <comment>
Setup parameters for local planner
</comment>

<call fn="setCheckResolution">
   <arg type="double">0.01</arg>
</call>

<call fn="setMinDurationForExtendPaths">
   <arg type="double">0.01</arg>
</call>

<call fn="setKeepFractionForExtendPaths">
   <arg type="double">0.9</arg>
</call>


</factory>
   </arg></call>
    
</factory>
      
 
   </arg></call> 
 
   <call fn="setMotionPlanner"><arg type="pointer">
      <factory instance="PRM">

   <call fn="setProximity"><arg type="pointer">
      <factory instance="GNATProximityKeyPointer">
</factory>


   </arg></call> 

   <call fn="setNrNeighborsSample"><arg type="int">15</arg></call>
   <call fn="setNrNeighborsQuery"> <arg type="int">50</arg></call>

   <call fn="allowCycles"><arg type="bool">false</arg></call>
 
   <call fn="setFractionSamplingTime">
      <arg type="double">0.2</arg>
   </call>
 
   <call fn="setMinMaxNrSamples">
      <arg type="int">2000</arg>
      <arg type="int">5000</arg>
   </call>
  
</factory>


   </arg></call>

   <call fn="setGMotionPlanner"><arg type="pointer">
      <factory instance="GPRM">
</factory>


   </arg></call>
  
</factory>

</arg>
</call>
</factory>'''

if __name__ == "__main__":
    main()
