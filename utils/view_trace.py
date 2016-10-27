import matplotlib.pyplot as plt
import numpy as np
import re 
import argparse



def parseATrace(filename):
    def comment_remover(text):
        def replacer(match):
            s = match.group(0)
            if s.startswith('/'):
                return " " # note: a space and not an empty string
            else:
                return s
        pattern = re.compile(
            r'//.*?$|/\*.*?\*/|\'(?:\\.|[^\\\'])*\'|"(?:\\.|[^\\"])*"',
            re.DOTALL | re.MULTILINE
        )
        return re.sub(pattern, replacer, text)
    print filename
    f = open(filename, 'r')
    parallelAccess = f.read()
    parallelAccess = comment_remover(parallelAccess)
    pattern  = '(A[\d+][\d+],)*A[\d+][\d+];'
    pattern = "A\[(\d+)\]\[(\d+)\]"
    p = re.compile(pattern)

    accessList = parallelAccess.split(";")
    parsedAccessList = []
    for access in accessList:
        if access  and (not access.isspace()):
            parsed = p.findall(access)
            parsedAccessList.append(map(lambda tuple: (int(tuple[0]),int(tuple[1])), parsed))

    return parsedAccessList



def find_plot_dimension(accessList):
    dim1 = 0
    dim2 = 0
    for parellelAccess in accessList:
	for access in parellelAccess:
	    print access
	    if dim1 < access[0]:
		dim1 = access[0]
	    if dim2 < access[1]:
		dim2 = access[1]
    return (dim1+1,dim2+1)

def create_plane_image(size):
	image = np.zeros(size[0]*size[1])
	image = image.reshape((size[0], size[1]))
	return image
	
def plot_array(image, item_to_access,title,coverage=[],coverage_id=-1,type=0):
    if coverage == [] or coverage_id != -1:
        for to_access in item_to_access:
            image[to_access[0]][to_access[1]]+=5

    if coverage != []:
        if coverage_id != -1:
            for accessed in coverage[coverage_id]:
                image[accessed[0]][accessed[1]]+=4
        else:
            for i in range(0,len(coverage)):
                for accessed in coverage[i]:
                    image[accessed[0]][accessed[1]]+=1

    #Remove item_to_access from image
    if type == 1:
        for to_access in item_to_access:
            image[to_access[0]][to_access[1]]=0
            
    row_labels = range(len(image))
    col_labels = range(len(image[0]))
    plt.rcParams['figure.figsize'] = 100, 100
    im=plt.matshow(image, cmap=plt.cm.jet)
    if coverage != [] and coverage_id == -1:
        plt.colorbar(im,ticks=[0, 1, 2,3],fraction=0.046, pad=0.04)
    plt.xticks(range(len(image[0])), col_labels)
    plt.yticks(range(len(image)), row_labels)
    plt.title(title)
    


    plt.gca().invert_yaxis()
    plt.gca().set_xticks([x - 0.5 for x in plt.gca().get_xticks()][1:], minor='true')
    plt.gca().set_yticks([y - 0.5 for y in plt.gca().get_yticks()][1:], minor='true')
    plt.grid(which='minor',color='b', linestyle='-', linewidth=2)

    plt.show()

if __name__ == "__main__":
    function_map = { 
	'viewTrace': plot_array,
	#'conv': convertFile,
    }
    parser = argparse.ArgumentParser()
    parser.add_argument( 'command', nargs=1 )
    parser.add_argument( 'fileName', nargs='+' )
    args= parser.parse_args()
    parallelAccesses = parseATrace(args.fileName[0]);
    dimensions = find_plot_dimension(parallelAccesses)
    rawImage = create_plane_image(dimensions)
    function = function_map[args.command[0]]
    function( rawImage, parallelAccesses[0], "ATrace View")
