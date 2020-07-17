from CodeSegment import CodeSegment
codeSegments = [CodeSegment(0x804e2a00, 0x804e3600), #after code menu, area before used for loading music
                CodeSegment(0x8055a600, 0x80563100), #after BOOST.gct, I gave a buffer of 20KB at the time of writing
                CodeSegment(0x805b5200, 0x805b61d0), #in system FW, in dynamically allocated area, but allocations are consistant and only happen during boot
                ]


initializersStartAddress = 0x80d00000
dataStartAddress = 0x8057d000
setupStartAddress = 0x804E2000


compilerSettings = "-g -fno-function-cse -ffunction-sections -fno-exceptions -fno-asynchronous-unwind-tables -std=c++17 -fno-builtin -ffreestanding -mcpu=750 -mmultiple -fno-inline -save-temps -fverbose-asm"
linkerSettings = f"-z common-page-size=4 -z max-page-size=4 --relax --section-start=.rodata={hex(dataStartAddress)} --gc-sections -nostdlib"

#not required
brawlFunctionMapPath = ''





includedCodesDirectory = "IncludedCodes"
outputDirectory = "Output"
disassemblyDirectory = "Disassembly"
intermediateFilesDirectory = "IntermediateFiles"


includedCodesFunctionListsDirectory = f"{intermediateFilesDirectory}/IncludedCodesFunctionLists"
editedCodesDirectory = f"{intermediateFilesDirectory}/EditedCodes"
editedCodesFunctionListsDirectory = f"{intermediateFilesDirectory}/EditedCodesFunctionLists"

initializersCPPPath = f"{intermediateFilesDirectory}/Initializers.cpp"
linkedInitializersPath = f"{intermediateFilesDirectory}/LinkedInitializers"
extractedDataPath = f"{intermediateFilesDirectory}/ExtractedData"
extractedInitializersPath = f"{intermediateFilesDirectory}/ExtractedInitializers"
compressedInitializersPath = f"{intermediateFilesDirectory}/CompressedInitializers"
dataAddressesFileForLinkerPath = f"{intermediateFilesDirectory}/DataAddressesForLinker.txt"
codesCPPPath = f"{intermediateFilesDirectory}/Codes.cpp"
linkedCodesPath = f"{intermediateFilesDirectory}/LinkedCodes"
setupCPPPath = f"{intermediateFilesDirectory}/Setup.cpp"
setupHeaderPath = f"{intermediateFilesDirectory}/Setup.h"
linkedSetupPath = f"{intermediateFilesDirectory}/LinkedSetup"
loadSetupFilePath = f"{intermediateFilesDirectory}/LoadSetup.s"
linkedLoadSetupFilePath = f"{intermediateFilesDirectory}/LinkedLoadSetup"
loadSetupBinaryPath = f"{intermediateFilesDirectory}/LoadSetup.bin"

finalDataPath = f"{outputDirectory}/Data.bin"
finalInitializersPath = f"{outputDirectory}/Init.bin"
finalSetupPath = f"{outputDirectory}/Setup.bin"
loadSetupASMPath = f"{outputDirectory}/LoadSetupFile.asm"

dataAddressesPath = f"{disassemblyDirectory}/DataAddresses.txt"
initializersAddressesPath = f"{disassemblyDirectory}/InitializersAddresses.txt"
initializersDisassemblyPath = f"{disassemblyDirectory}/InitializersDisassembly.txt"
initializersMapPath = f"{disassemblyDirectory}/Initializers.map"
codesAddressesPath = f"{disassemblyDirectory}/CodesAddresses.txt"
codesDisassemblyPath = f"{disassemblyDirectory}/CodesDisassembly.txt"
codesMapPath = f"{disassemblyDirectory}/Codes.map"
setupAddressesPath = f"{disassemblyDirectory}/SetupAddresses.txt"
setupDisassemblyPath = f"{disassemblyDirectory}/SetupDisassembly.txt"
setupMapPath = f"{disassemblyDirectory}/Setup.map"




ppcObjCopy = "powerpc-eabi-objcopy.exe"
ppcNM = "powerpc-eabi-nm.exe"
ppcCompiler = "powerpc-eabi-gcc.exe"
ppcLinker = "powerpc-eabi-ld.exe"
ppcObjDump = "powerpc-eabi-objdump.exe"