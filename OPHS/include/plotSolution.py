import matplotlib.pyplot as plt
import pandas as pd
import random as random
import sys

def addEuclideanArrow(ax1, xInicio, yInicio, xFim, yFim, cor, legenda, lineStyle='-'):
    if legenda == "":
        ax1.arrow(xInicio, yInicio,(xFim-xInicio) , (yFim-yInicio), width = 0.001, head_width = 0.15, head_length = 0.15, length_includes_head=True, color=cor, linestyle=lineStyle)
    else:
        ax1.arrow(xInicio, yInicio,(xFim-xInicio) , (yFim-yInicio), width = 0.001, head_width = 0.15, head_length = 0.15, length_includes_head=True, color=cor, label=legenda, linestyle=lineStyle)

def plotSolution(inst, sol):
    
    # INSTÂNCIA
    f = open(inst, "r")
    f.seek(0)
    
    # lê primeira linha
    line = f.readline().rstrip('\n').split('\t')
    numVertex = int(line[0])
    numExtraHotels = int(line[1])
    numTrips = int(line[2])
    
    # lê segunda linha
    totalTourLength = float(f.readline())
    
    # lê a terceira linha
    line = f.readline().rstrip("\n").split("\t")
    maxTripLength = []
    for i in range(numTrips):
        maxTripLength.append(float(line[i]))

    # lê nós
    id = []
    x = []
    y = []
    score = []
    
    f.readline()
    
    for i in range(numVertex+numExtraHotels):
        line = f.readline().rstrip("\n").split("\t")
        id.append(i)
        x.append(float(line[0]))
        y.append(float(line[1]))
        score.append(line[2])
        
    f.close()
    
    fig = plt.figure()
    ax1 = fig.add_subplot()
    ax = plt.gca()
    
    rangeX = max(x)-min(x)
    rangeY = max(y)-min(y)

    if(rangeX > rangeY):
        space = rangeX - rangeY
        plt.xlim([min(x)-5, max(x)+5])
        plt.ylim([min(y)-space/2 - 5, max(y)+space/2 + 5])
    else:
        space = rangeY - rangeX
        plt.ylim([min(y)-5, max(y)+5])
        plt.xlim([min(x)-space/2 - 5, max(x)+space/2 + 5])
    
    for location in range(numExtraHotels+2, numVertex+numExtraHotels-1):
        plt.plot(x[location], y[location], 'o', color='pink')
        
    for hotel in range(0, numExtraHotels+1):
        plt.plot(x[hotel], y[hotel], 'o', color='aqua')
    
    for i in range(len(x)-1):
    
        # nomeando os locais por id
        if (score[i] == 0):
            ax.annotate(id[i], xy=(x[i],y[i]-0.1), size=6, horizontalalignment='center', verticalalignment='bottom')
        else:
            ax.annotate(id[i], xy=(x[i],y[i]-0.1), size=6, horizontalalignment='center', verticalalignment='top')
    
    
    # SOLUÇÃO
    
    f0 = open(sol, "r")
    
    for i in range(numTrips):
        line = f0.readline()[:-1].split(" ")
        
        r = random.random()*i%1 
        g = random.random()*i%1
        b = random.random()*i%1
        
        color = (r,g,b)
        
        for j in range(len(line)-2):
            inicio = int(line[j])
            fim = int(line[j+1])
            
            if fim==numVertex:
                fim=0
                
            if inicio==numVertex:
                inicio=0
                
            if(j==0):
                addEuclideanArrow(ax1,x[inicio],y[inicio],x[fim],y[fim],color,'Trip ' + str(i+1))
            else:
                addEuclideanArrow(ax1,x[inicio],y[inicio],x[fim],y[fim],color,'')
        
        
    plt.legend()
    result = "Solução"
    plt.title(result)

if __name__ == '__main__':
    instance = sys.argv[1]
    solution = sys.argv[2]
    
    plotSolution(instance, solution)
    fig = plt.gcf()
    # plt.show()
    # plt.draw()
    
    path = solution.split("./../output/")[1]
    path = path.split(".txt")[0]
    path = "./../plots/" + path + ".png"
        
    fig.savefig(path)