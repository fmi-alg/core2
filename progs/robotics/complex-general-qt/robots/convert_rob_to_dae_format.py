# This script convert our .rob format to .dae format which can be input to the ompl_app
# NOTE: Open Asset Import Library (assimp) is required
#       1. Build the assimp from here: https://github.com/assimp/assimp
#       2. Add its `bin' directory to PATH
#
# Usage: python convert_rob_to_dae_format.py <my_robot.rob> <R>
#   argument my_robot.rob is the input robot file
#   argument R is the robot radius -- the scale factor of the robot
# Output: my_robot.dae
#
import sys
import math
import subprocess

if (len(sys.argv) > 1):
  robot_file = sys.argv[1]
R0 = 1.0
if (len(sys.argv) > 2):
  R0 = float(sys.argv[2])

off_file = robot_file.replace('.rob', '.off')
dae_file = robot_file.replace('.rob', '.dae')

verts = []
faces = []
nv, nf = 0, 0
maxdist = 0


with open(robot_file) as f:
  lines = f.readlines()

for line in lines:
  line = line.strip()
  if len(line)==0 or line[0]=='#':
    continue
  if nv == 0:
    nv = int(line)
    continue
  if (len(verts) < nv):
    vert = [float(x) for x in line.split()]
    maxdist = max(maxdist, math.sqrt(vert[0]*vert[0] + vert[1]*vert[1]))
    vert.append(0.0)
    verts.append(vert)
    continue
  if nf == 0:
    nf = int(line)
    continue
  if (len(faces) < nf):
    face = [int(x) for x in line.split()]
    faces.append([len(face)] + face)

if nf == 0:
  # starshaped robot with triangles implicitly defined
  nv = nv + 1
  verts = [[0,0,0]] + verts
  nf = nv - 1
  for i in range(nf - 1):
    faces.append([3, 0, i+1, i+2])
  faces.append([3, 0, i+2, 1])

if maxdist > 0:
  scale = R0/maxdist

f = open(off_file,'w')
print >> f, 'OFF'
print >> f, nv, nf, 0
for r in verts:
  print >> f, ' '.join([str(x*scale) for x in r])
for r in faces:
  print >> f, ' '.join([str(x) for x in r])
f.close()

subprocess.call(["assimp", "export", off_file, dae_file])
#subprocess.call(["rm", off_file])
