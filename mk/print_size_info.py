from sys import argv
from os.path import exists


def getTypeMeaning(type):
    return {
        "A": "Absolute symbol, global",
        "a": "Absolute symbol, local",
        "B": "Uninitialized data (bss), global",
        "b": "Uninitialized data (bss), local",
        "D": "Initialized data (bbs), global",
        "d": "Initialized data (bbs), local",
        "F": "File name",
        "l": "Line number entry (see the –a option)",
        "N": "No defined type, global. This is an unspecified type, compared to the undefined type U.",
        "n": "No defined type, local. This is an unspecified type, compared to the undefined type U.",
        "S": "Section symbol, global",
        "s": "Section symbol, local",
        "T": "Text symbol, global",
        "t": "Text symbol, local (static)",
        "U": "Undefined symbol",
    }[type]


def main():
    if len(argv) < 2:
        print("need a input file")
        return

    file = argv[1]
    if not exists(file):
        print(f"\"{file}\" is not a file or does not exist")
        return

    if not file.endswith(".symbol_info"):
        print(f"\"{file}\" is not a valid file for this script")
        return

    processedInfo = {}
    with open(file, 'r') as f:
        lines = f.readlines()
        for line in lines:
            infos = line.split()
            symbolAddress = infos[0]
            symbolSize = int(infos[1], 16)
            symbolType = infos[2]
            symbol = infos[3]

            if len(infos) > 4:
                symbolFile = infos[-1].split(':')[0]
            else:
                # print(infos)
                symbolFile = "Symbols size in unknown files"

            if '/' not in symbolFile:
                # print(infos)
                symbol = ' '.join(infos[3:])
                symbolFile = "Symbols size in unknown files"

            if symbolFile not in processedInfo:
                processedInfo[symbolFile] = {}
            if symbolType not in processedInfo[symbolFile]:
                processedInfo[symbolFile][symbolType] = []
            processedInfo[symbolFile][symbolType].append({'symbol': symbol, 'size': symbolSize})

    printInfos = {}
    totalSize = 0
    for fileName in processedInfo:
        printInfos[fileName] = {}
        fileSize = 0
        for type in processedInfo[fileName]:
            typeSize = 0
            # processedInfo[fileName][type] is a list of symbol objects like:
            # [{"symbol": printf, "size": 722}, {"symbol": main, "size": 231}]
            for typeDict in processedInfo[fileName][type]:
                fileSize += typeDict["size"]
                typeSize += typeDict["size"]
            printInfos[fileName][type] = typeSize
        printInfos[fileName]["subtotal"] = fileSize
        totalSize += fileSize

    # print(printInfos.items())
    # return
    indent = "  "
    sortedPrintInfos = dict(sorted(printInfos.items(), key=lambda item: item[1]["subtotal"]))
    for fileName in sortedPrintInfos:
        print(fileName + ':')
        for type in sortedPrintInfos[fileName]:
            if type == "subtotal":
                continue
            print(indent + getTypeMeaning(type) + ', size:', end=' ')
            print(sortedPrintInfos[fileName][type])
            for typeDict in processedInfo[fileName][type]:
                print(indent + indent + typeDict["symbol"] + ', size:', end=' ')
                print(typeDict["size"])

        print(indent + "subtotal:", end=' ')
        print(sortedPrintInfos[fileName]["subtotal"])
    print()
    print("Total:", totalSize)
    # for debug:
    # import json
    # print(json.dumps(processedInfo))


if __name__ == '__main__':
    main()
