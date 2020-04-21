# This script convert the .txt format environment to .dae format which can be input to the ompl_app
# NOTE: Open Asset Import Library (assimp) is required
#       1. Build the assimp from here: https://github.com/assimp/assimp
#       2. Add its `bin' directory to PATH
#
# Usage: python convert_txt_to_dae_format.py <my_envir.txt>
#   argument my_envir.txt is the input environment file
# Output: my_envir.dae
#
# IMPORTANT: 
#   The polygons in the envir file must be defined by the 
#   CCW order of its vertices (required by OMPL). 
#   Some environments defines the exterior of a polygon 
#   by the CW order of the vertices will not be recognized
#   correctly by OMPL!
#   Each point/polygon is defined in one line.
import sys
import math
import subprocess

if (len(sys.argv) > 1):
  envir_file = sys.argv[1]


off_file = envir_file.replace('.txt', '.off')
dae_file = envir_file.replace('.txt', '.dae')

verts = []
faces = []
nv, nf = 0, 0

with open(envir_file) as f:
  lines = f.readlines()

for line in lines:
  line = line.split('#')[0].strip()
  if len(line)==0:
    continue
  if nv == 0:
    nv = int(line)
    continue
  if (len(verts) < nv):
    vert = [float(x) for x in line.split()]
    vert.append(0.0)
    verts.append(vert)
    continue
  if nf == 0:
    nf = int(line)
    continue
  if (len(faces) < nf):
    face = [int(x) for x in line.split()]
    faces.append([len(face)] + face)

f = open(off_file,'w')
print >> f, 'OFF'
print >> f, nv, nf, 0
for r in verts:
  print >> f, ' '.join([str(x) for x in r])
for r in faces:
  print >> f, ' '.join([str(x-1) for x in r[:-1]])
f.close()

subprocess.call(["assimp", "export", off_file, dae_file])
subprocess.call(["rm", off_file])
