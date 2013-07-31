#This script requires a text file DataSetsForClassification.dat which represents the files you'd like to segment.
#The results of this segmentation is saved to the directory dataSets. In addition to that the names of the segmented files
#will be written to the file adjustedDataSets.dat and also models.dat. The purpose of this is to make using this code
#with Jordan Frank's TDE implementation simpler.

import os

numberOfSections = raw_input( "How many training sections would you like to break file into?" )

answer = raw_input( "Would you like to use the sliding window technique? (Y/N) " )

if answer == "N":

  numberOfTestSections = raw_input( "How many test sections would you like? " )

if answer == "Y":

  numberOfDataPoints = raw_input( "How many data points would you like for each file " )
  
dataSetsFileName = str( raw_input ( "What is the name of your data set?" ) ) #Reads in list of files to segments

outputFile = "adjustedDataSets.dat" #The file representing the segmented results
testOutputFile = "DataSetsForClassification.dat" #Files to be used for classification

modelsOutputFile = "models.dat" #the model files output

dataFileIncrementer = 0
 
tmpDataSetsFile = open( dataSetsFileName, "r" ) #used the find the size of the array
numberOfOutputFiles = len( tmpDataSetsFile.readlines( ) ) * numberOfSections

dataSetsOutput = open( outputFile, "w" )
dataToClassifyOutput = open( testOutputFile, "w" )

modelsOutput = open( modelsOutputFile, "w" )

dataSetsFile = open( dataSetsFileName, 'r' )

os.chdir("dataSets")

for line in sorted( dataSetsFile.readlines( ) ):
  dataFile = open( (line + ".dat").replace("\n",""), 'r' )
   
  numberOfPoints = len( dataFile.readlines() )
  dataFile.seek( 0 ) 
   
  #answer = str( raw_input( "Would you like to use the sliding window technique? (Y/N) " ) ) 
   
  for i in xrange( 0, int( numberOfSections )  ):
  
    output = open( ( line + "-" + str( i ) + ".dat " ).replace( "\n","" ), 'w' )
    
    for j in xrange( 0, int( numberOfPoints ) /  ( int( numberOfSections ) + 1 ) ):

      output.write( dataFile.readline( ) )
      
    dataSetsOutput.write( "dataSets\\" + ( line + "-" + str( i ) ).replace( "\n", "" ) + "\n" ) 
    
    os.chdir( "../" )
    modelsOutput.write( "dataSets\\" + ( line + "-" + str( i ) + "-model.dat" ).replace( "\n", "" ) + "\n" )
    os.chdir( "dataSets" )

  if answer == "N":
    
    for i in xrange( 0, int( numberOfTestSections )  ):
    
      if line == "stand" :
      
        l = "stnd"

      else:
      
        l = line
        
      output = open( ( l + "-test-" + str( i ) + ".dat " ).replace( "\n","" ), 'w' )
      
      for j in xrange( 0, ( int( numberOfPoints ) - int( numberOfPoints ) / 2 ) /  ( int( numberOfTestSections ) ) ):

        output.write( dataFile.readline( ) )
      
      dataToClassifyOutput.write( (l + "-test-" + str( i ) + " ").replace( "\n", "" ) )
      
  else:
  
    dataFiles = dataFile.readlines( )
    
    #print dataFiles
    
    startIndex = 0
    endIndex = int( numberOfDataPoints )
    
    #Increments everytime I write out a file name for a format like: test-0, test-1, test-2
    fileIncrementer = 0
    
    numberOfLinesLeft = len( dataFiles )
    
    #Create data files while there is enough remaining points
    while( endIndex <= len( dataFiles ) ):
    
      output = open( ( line + "-test-" + str( fileIncrementer ) + ".dat " ).replace( "\n","" ), 'w' )
      
      for i in xrange( startIndex, endIndex ):
      
       # print i
        output.write( dataFiles[ i ] )
        
        numberOfLinesLeft -= 1
      
      dataToClassifyOutput.write( (line + "-test-" + str( fileIncrementer ) + " ").replace( "\n", "" ) )
      fileIncrementer += 1
      
      startIndex += 1
      endIndex += 1
      
    
modelsOutput = open ( modelsOutputFile, "a" )
 
modelsOutput.write( "t" )
