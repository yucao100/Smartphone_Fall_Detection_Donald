#A simple script to parse the USC data into files of acceleration magnitudes

import math

fileList = [ ]

filesToFormat = open( 'USC_Datasets.dat', 'r' )

formattedFileNamesOutput = open( 'USC_Formatted_Data.dat', 'w' )

formattedFileOutput = None

for line in filesToFormat.readlines( ):

  formattedFileOutput = open( 'dataSets\\' + line[ 0 : 4 ] + '-MagnitudeData', 'w' )
  
  for item in open( "dataSets/" + line.replace( '\n', '' ), 'r' ).readlines( ):
  
    magnitudeGyroData = item.split( ' ' )
    
    formattedFileOutput.write( str( math.sqrt( math.pow( float( magnitudeGyroData[ 0 ] ), 2) + math.pow( float( magnitudeGyroData[ 1 ] ), 2) + math.pow( float( magnitudeGyroData[ 2 ] ), 2) ) )  + "\n" )