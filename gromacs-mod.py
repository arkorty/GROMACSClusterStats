import sys
import numpy


def loadFromText(frVsClPath: str, frVsSoVsWgPath: str):
    frVsCl = numpy.loadtxt(frVsClPath)  # loading data from text file
    cl = frVsCl[:, 1]  # ndarray containing clusters

    frVsSoVsWg = numpy.loadtxt(frVsSoVsWgPath)  # loading data from text file
    wg = frVsSoVsWg[:, 2]  # ndarray containing weights

    return cl, wg


def compute(cl, wg):
    numCl = int(numpy.max(cl))  # maximum number of clusters
    numFr = int(len(wg))  # number of frames
    maxWgIdx = int(numpy.argmax(wg))  # index of the maximum value in the list

    # creating zero-filled ndarrays
    nReWgCl = numpy.zeros(numCl + 1)
    reWgClust = numpy.zeros(numCl + 1)
    norReWgCl = numpy.zeros(numCl + 1)

    # crunching some numbers
    for i in range(numCl + 1):
        for j in range(numFr):
            if cl[j] == i:
                nReWgCl[i] += 1
                reWgClust[i] += wg[i]

    # doing some normalization
    reWgClSum = sum(reWgClust)
    for i in range(1, numCl + 1):
        norReWgCl[i] = (reWgClust[i] / reWgClSum) * numFr

    return maxWgIdx, nReWgCl, reWgClust, norReWgCl


def output(maxWgIdx, wg, nReWgCl, reWgClust, norReWgCl):
    print(
        "maximum contributing frame is "
        + str(maxWgIdx)
        + " with a weight of "
        + "{:.3f}".format(wg[maxWgIdx])
    )

    print("---")  # for formatting

    for i in range(1, len(nReWgCl)):
        print(str(i) + "\t" + str(int(nReWgCl[i])) + "\t" + "{:.3f}".format(reWgClust[i]))

    print("---")  # for formatting

    for i in range(1, len(norReWgCl)):
        print(str(i) + "\t" + str(int(nReWgCl[i])) + "\t" + "{:.3f}".format(norReWgCl[i]))


# this is where the program starts
if __name__ == "__main__":
    # this runs everything else
    if len(sys.argv) == 3:
        cl, wg = loadFromText(sys.argv[1], sys.argv[2])
        maxWgIdx, nReWgCl, reWgClust, norReWgCl = compute(cl, wg)
        output(maxWgIdx, wg, nReWgCl, reWgClust, norReWgCl)

    # this prints a help message
    elif len(sys.argv) == 2 and (sys.argv[1] == "-h" or sys.argv[1] == "--help"):
        print(
            'usage: "python3 '
            + str(sys.argv[0])
            + ' /path/to/f_vs_c.txt /path/to/weights.txt"'
        )
