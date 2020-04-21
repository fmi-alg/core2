import random
import os.path

TRI_SIZE_LIMIT_X = 64
TRI_SIZE_LIMIT_Y = 64

def randPoint2D(center):
    return random.randint(center[0] - TRI_SIZE_LIMIT_X/2, center[0] + TRI_SIZE_LIMIT_X/2),\
            random.randint(center[1] - TRI_SIZE_LIMIT_Y/2, center[1] + TRI_SIZE_LIMIT_Y/2)
    
#http://www.cs.princeton.edu/~rs/AlgsDS07/16Geometric.pdf
def CCW(A, B, C):
    det = (B[0] - A[0])*(C[1] - A[1]) - (B[1] - A[1])*(C[0] - A[0])
    if det > 0:
        return A, B, C
    if det < 0:
        return A, C, B
    if det == 0:
        return False

tris = []
nT = int(raw_input('choose number of triangles: '))
while True:
    ctr = random.randint(0 + TRI_SIZE_LIMIT_X/2, 511 - TRI_SIZE_LIMIT_X/2), \
            random.randint(0 + TRI_SIZE_LIMIT_Y/2, 511 - TRI_SIZE_LIMIT_Y/2)
    A = randPoint2D(ctr)
    B = randPoint2D(ctr)
    C = randPoint2D(ctr)
    if CCW(A, B, C):
        A, B, C = CCW(A, B, C)
        tris.append( (A, B, C) )
    if len(tris) == nT:
        break
#print tris

i = 1
while os.path.exists('input' + str(i) +'.txt'):
    i += 1
out = open('input' + str(i) +'.txt', 'w')
print >> out, len(tris)*3
for t in tris:
    for v in t:
        print >> out, v[0], v[1]
print >> out, nT
for j in range(len(tris)):
    print >> out, j*3+1, j*3+2, j*3+3, j*3+1
print 'input' + str(i) +'.txt generated'