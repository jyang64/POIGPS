import re
import sys

def getVal(key, line):
    # grab key-value pair
    match = re.search(key+"\s*?=\s*?\".*?\"", line)
    assert match
    val = re.sub(key+"\s*?=\s*?\"", "", match.group())
    return re.sub("\"$", "", val).strip()

def printInfo(node):
    if len(node) > 1:
        info = {"name": "", "amenity":""}
        info["lat"] = getVal("lat", node[0])
        info["lon"] = getVal("lon", node[0])
        for i in range(1, len(node)):
            if '"name"' in node[i]:
                info["name"] = getVal("v", node[i])
            elif '"amenity"' in node[i]:
                info["amenity"] = getVal("v", node[i])
        # print values
        print('{} {} {} - {}'.format(info["lat"], info["lon"], info["name"].
        replace('"','\''), info["amenity"].replace('"','\'')))

## parse file specified as command line argument
with open(sys.argv[1], "rt") as fid:
    node = []
    for line in fid:
        line = line.strip().lower()
        # found start node
        if line.startswith("<node"):
            if not line.endswith("/>"):
                node = [line]
        # found end node
        elif line.startswith("</node"):
            printInfo(node)
            node = []
        # found a tag
        elif node:
            if line.startswith("<tag") and ('"name"' in line or '"amenity"' in line):
                node.append(line)
