import os;
import subprocess;
import sys;

X_MIN = -2
X_MAX = 2
Y_MIN = -2
Y_MAX = 2
# OPERATORS = ["C_EVAL", "C_EVAL_NI" ]
# THRESH = [0, 2, 4, 8, 16, 32 , 64]
SEEDS = [691449, 506392, 562483] # 16542,  807751, 951739, 321581, 912418, 817699, 358692] # 872163, 291968, 675539, 722352, 189574, 455151, 355363, 478596, 808554, 381391, 400176, 548797, 60258, 745216, 145657, 761106, 287266, 108960, 220125, 384086, 109161, 173962, 488178, 970842, 178685, 118314, 265996, 473102, 195901, 980052, 761282, 918795, 799626, 102547, 913412, 348659, 154643, 125547, 666754]

COLLECT_STATS ={"iters" : 1, "time":1, "ambiguous":1}
OPERATORS = ["ceval", "ceval_ni"]
  
def main():
  global_stats = {}
  for operator in OPERATORS:
    collection="../../data/Collections/friscoRealMini"
    if len(sys.argv)==1:
       print 'Usage run_frisco polynomialCollection'
    else:
       collection=sys.argv[1]

    #for f in os.listdir("./data"):
    for line in open(collection):
      li = line.strip()
      if not li.startswith("#"):
          print line.rstrip()
          file_name = os.path.join("../../data/frisco", line.rstrip());
          if os.path.isfile(file_name):
            file = open(file_name)
            poly_name = file.readline().strip("! \n")
            file.close()

            args = []
            args.append("./main_ceval")
            #args.append("-%s" % operator)
            args.append("--poly %s" % file_name)
            #args.append("--x_min %s" % X_MIN)
            #args.append("--x_max %s" % X_MAX)
            #args.append("--y_min %s" % Y_MIN)
            #args.append("--y_max %s" % Y_MAX)
            args.append("--use_root_bounds")
            #args.append("--min_box_size 0.0001")
            args.append("--min_box_size 0.01")
            if operator == "ceval_ni":
              args.append("--no_use_inclusion")
            print args

            output=subprocess.check_output(args, stderr=subprocess.STDOUT)
            output.strip(" \n")
            stats = output.split(",")
            stats_map = {}
            for stat in stats:
              kv = stat.split("=")
              print kv
              stats_map[kv[0]] = {}
              stats_map[kv[0]][operator] = int(kv[1])

            if poly_name in global_stats:
              for k,v in stats_map.iteritems():
                global_stats[poly_name][k].update(v)
            else:
              global_stats[poly_name] = stats_map

       
  #print global_stats

  len_seeds = len(SEEDS)
    
  for k,v in global_stats.iteritems():
    output = ''
    output += k + " & "
    for k1, v1 in v.iteritems():
      if k1 in COLLECT_STATS:
        output += '('+ k1 + ')'
        for operator in OPERATORS:
          output += '[' + operator + ']'
          val = v1[operator]
          if k.find('random') == 0:
            val = float(v1[operator]) / len_seeds
          output += str(val) + ' & '
    output += '\\\\'
    print output

    

if __name__ == "__main__":
  main()
