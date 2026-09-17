# Point at the running GUI and measure what it actually drew.
#
#   tools/gui-probe.ps1 -List
#   tools/gui-probe.ps1 -Hover cboxHashType
#   tools/gui-probe.ps1 -Hover bHashCopy,fixGptCheckBox -Shot tip.png
#
# -List prints every widget: its object name, the text on it, and where it is on
# screen. The object names are the ones in src/mainwindow.ui, so -Hover takes
# the same names.
#
# -Hover moves the pointer onto each named widget in turn, waiting on each long
# enough for a tooltip to appear, and reports the tooltip showing at the end
# along with its measured width. -Shot saves a picture of the window and a
# margin around it, which is where tooltips end up.
#
# Positions come from the application itself over UI Automation, which Qt speaks
# on Windows, and are read again before every move. Working them out from a
# screenshot instead leaves the pointer landing on the desktop as soon as the
# window moves, which is exactly how this script came to be written.
#
# The measured width is the point. A tooltip is clipped when its box is narrower
# than its text needs, and no screenshot tells you that to the pixel. Naming
# several widgets in one run walks the pointer from one to the next while the
# first tooltip is still up, which is when Qt reuses a single tooltip label for
# the next tooltip -- the case where sizing has gone wrong before.
#
# Needs no admin rights and touches no device. Build with "tools/build.sh test"
# first so that starting the application does not raise a UAC prompt.
#
# Copyright (C) 2026 peacepenguin, GPL-2.0-or-later.

param(
    # Widget object names to hover, in order, comma separated.
    [string]$Hover = "",
    # Save a screenshot here when the run ends.
    [string]$Shot = "",
    # List the widgets and stop.
    [switch]$List,
    # Milliseconds to rest on each widget, and on the last one before measuring.
    [int]$Dwell = 1400,
    [int]$Hop = 2000,
    # Somewhere with no tooltip of its own to start from, so the first widget in
    # the list is arrived at rather than already under the pointer.
    [string]$Park = "progressGroup",
    [string]$Process = "WinDiskImager"
)

$ErrorActionPreference = "Stop"
Add-Type -AssemblyName UIAutomationClient, UIAutomationTypes, System.Drawing

Add-Type @'
using System; using System.Runtime.InteropServices;
public class GuiProbe {
  [DllImport("user32.dll")] public static extern bool SetCursorPos(int X, int Y);
  [DllImport("user32.dll")] public static extern bool SetProcessDPIAware();
  [DllImport("user32.dll")] public static extern bool SetForegroundWindow(IntPtr h);
}
'@

# Without this every coordinate is in the scaled units Windows hands an unaware
# process, and the pointer misses by the scaling factor on any display that is
# not at 100%.
[void][GuiProbe]::SetProcessDPIAware()

$proc = Get-Process $Process -ErrorAction SilentlyContinue | Select-Object -First 1
if ($null -eq $proc) {
    [Console]::Error.WriteLine("error: $Process is not running. Start build\$Process.exe first.")
    exit 1
}

$root = [System.Windows.Automation.AutomationElement]::RootElement
$byProcess = New-Object System.Windows.Automation.PropertyCondition(
    [System.Windows.Automation.AutomationElement]::ProcessIdProperty, $proc.Id)

# By window handle, not by process: a tooltip on screen is also a top-level
# window of this process, and asking for "a window of this process" will
# cheerfully hand back the tooltip instead of the application.
function Get-Window {
    $w = [System.Windows.Automation.AutomationElement]::FromHandle($proc.MainWindowHandle)
    if ($null -eq $w) { throw "no window for handle $($proc.MainWindowHandle)" }
    return $w
}

function Get-Widgets {
    $map = @{}
    $all = (Get-Window).FindAll([System.Windows.Automation.TreeScope]::Descendants,
                                [System.Windows.Automation.Condition]::TrueCondition)
    foreach ($e in $all) {
        $id = $e.Current.AutomationId
        if ([string]::IsNullOrEmpty($id)) { continue }
        # Qt reports the whole path, e.g. QApplication.MainWindow.centralwidget
        # .hashGroup.bHashGen; the last part is the object name from the .ui.
        $map[($id -split '\.')[-1]] = $e.Current
    }
    if ($map.Count -eq 0) {
        # An ordinary build asks for elevation, and a script that is not itself
        # elevated cannot read the interface of one that got it: the tree comes
        # back empty rather than refused, which looks like nothing at all went
        # wrong. That is the usual reason by far, so say so.
        [Console]::Error.WriteLine(
            "error: $Process shows no widgets. Most likely it is an ordinary build, " +
            "which runs elevated, and this script is not. Build the test one with " +
            "'tools/build.sh test' and start build\$Process.exe again.")
        exit 1
    }
    return $map
}

if ($List) {
    $widgets = Get-Widgets
    foreach ($name in ($widgets.Keys | Sort-Object)) {
        $r = $widgets[$name].BoundingRectangle
        "{0,-24} {1,-22} x={2,-5} y={3,-5} {4}x{5}" -f $name, $widgets[$name].Name,
            [int]$r.X, [int]$r.Y, [int]$r.Width, [int]$r.Height
    }
    exit 0
}

if ([string]::IsNullOrWhiteSpace($Hover)) {
    [Console]::Error.WriteLine("usage: gui-probe.ps1 -List | -Hover <widget[,widget...]> [-Shot file.png]")
    exit 2
}

try { (New-Object -ComObject WScript.Shell).AppActivate($proc.Id) | Out-Null } catch {}
[void][GuiProbe]::SetForegroundWindow($proc.MainWindowHandle)
Start-Sleep -Milliseconds 400

function Move-To([string]$name) {
    $widgets = Get-Widgets          # again each time: the window may have moved
    if (-not $widgets.ContainsKey($name)) {
        [Console]::Error.WriteLine("error: no widget named '$name'. Run with -List to see the names.")
        exit 1
    }
    $r = $widgets[$name].BoundingRectangle
    [void][GuiProbe]::SetCursorPos([int]($r.X + $r.Width / 2), [int]($r.Y + $r.Height / 2))
}

Move-To $Park
Start-Sleep -Milliseconds 500

$names = $Hover -split ','
for ($i = 0; $i -lt $names.Count; $i++) {
    Move-To $names[$i].Trim()
    if ($i -lt $names.Count - 1) { Start-Sleep -Milliseconds $Dwell }
    else { Start-Sleep -Milliseconds $Hop }
}

# The tooltip is the top-level window of this process that is not the window.
$found = $false
foreach ($t in $root.FindAll([System.Windows.Automation.TreeScope]::Children, $byProcess)) {
    if ($t.Current.NativeWindowHandle -eq [int]$proc.MainWindowHandle) { continue }
    $r = $t.Current.BoundingRectangle
    if ($r.Width -le 0) { continue }
    "tip w={0} h={1} at {2},{3}: {4}" -f [int]$r.Width, [int]$r.Height,
        [int]$r.X, [int]$r.Y, $t.Current.Name
    $found = $true
}
if (-not $found) {
    # Usually the previous tooltip is covering the next widget, so the pointer
    # landed on the tooltip rather than on the widget and nothing new was asked
    # for. Hover the widgets in a different order, or one at a time.
    "no tooltip showing"
}

if (-not [string]::IsNullOrWhiteSpace($Shot)) {
    $w = (Get-Window).Current.BoundingRectangle
    $pad = 30
    $margin = 300           # tooltips are drawn outside the window as often as in
    $bmp = New-Object System.Drawing.Bitmap ([int]$w.Width + $pad * 2 + $margin),
                                            ([int]$w.Height + $pad * 2 + $margin)
    $g = [System.Drawing.Graphics]::FromImage($bmp)
    $g.CopyFromScreen([int]$w.X - $pad, [int]$w.Y - $pad, 0, 0, $bmp.Size)
    $path = $Shot
    if (-not [System.IO.Path]::IsPathRooted($path)) {
        $path = Join-Path (Get-Location).Path $path
    }
    $path = [System.IO.Path]::GetFullPath($path)
    $bmp.Save($path, [System.Drawing.Imaging.ImageFormat]::Png)
    $g.Dispose(); $bmp.Dispose()
    "saved $path"
}
