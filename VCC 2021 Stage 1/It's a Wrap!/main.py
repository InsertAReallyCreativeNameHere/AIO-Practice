class std:
    @staticmethod
    def accumulate(container):
        ret = 0
        for it in container:
            ret += it
        return ret;

def CalculatePackageSurfaceArea(string: str):
    arr = string.split('x')
    arr = [ int(arr[0]), int(arr[1]), int(arr[2]) ]
    return 2 * (arr[0] * arr[1] + arr[0] * arr[2] + arr[1] * arr[2])

def GetParcelDimensionsFromFile(fileName: str):
    return [float(CalculatePackageSurfaceArea(string)) for string in open(fileName).read().split('\n')]

def main():
    print("In total, we need ", std.accumulate(GetParcelDimensionsFromFile("parcels.txt")) / 10000, "m^2 of plastic.")

main()