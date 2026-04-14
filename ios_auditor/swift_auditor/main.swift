import Foundation

func run(_ args: [String]) -> String {
    let proc = Process()
    proc.executableURL = URL(fileURLWithPath: "/usr/bin/env")
    proc.arguments = ["adb"] + args
    let pipe = Pipe()
    proc.standardOutput = pipe
    proc.standardError = pipe
    do {
        try proc.run()
    } catch {
        return "adb not found or failed to run"
    }
    let data = pipe.fileHandleForReading.readDataToEndOfFile()
    return String(data: data, encoding: .utf8) ?? ""
}

let args = CommandLine.arguments
if args.count == 1 {
    print("Usage: swift_auditor [--devices] [--list-packages] [--third-party] [--permissions PKG] [--net]")
    exit(1)
}

var i = 1
var listPackages = false
var thirdParty = false
var devices = false
var net = false
var pkg: String? = nil

let handlers: [String: () -> Void] = [
    "--list-packages": { listPackages = true },
    "--third-party":   { thirdParty = true },
    "--devices":       { devices = true },
    "--net":           { net = true }
]

while i < args.count {
    let a = args[i]
    if let h = handlers[a] {
        h()
    } else if a == "--permissions" && i + 1 < args.count {
        pkg = args[i + 1]
        i += 1
    } else {
        print("Unknown arg: \(a)")
    }
    i += 1
}

if devices { print(run(["devices", "-l"])) }
if listPackages {
    if thirdParty { print(run(["shell", "pm", "list", "packages", "-3"])) }
    else { print(run(["shell", "pm", "list", "packages"])) }
}
if let p = pkg { print(run(["shell", "dumpsys", "package", p])) }
if net {
    var out = run(["shell", "ss", "-tunp"]) 
    if out.isEmpty { out = run(["shell", "netstat", "-anp"]) }
    if out.isEmpty { fputs("Could not obtain network connections (ss/netstat missing).\n", stderr) }
    else { print(out) }
}
