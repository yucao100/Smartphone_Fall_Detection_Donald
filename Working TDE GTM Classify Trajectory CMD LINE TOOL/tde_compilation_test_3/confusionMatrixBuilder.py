import os
import glob

class ClassificationResult:

  def __init__( self ):
  
    self.similarityScore = 0
    self.dataSetTrueAnnotation = ""
    self.dataSetClassifiedAnnotation = ""
    
#Possible classifications are running, biking, and walking (RBS). The results of this algorithm are very
#dependent on the format of the classification results. It is assuming they are stored as running results, biking results, and walking results.    
    
class ConfusionMatrix:

  def __init__( self ):

    self.confusionMatrix = [ [ 0 for col in range( 3 ) ] for row in range( 3 ) ]
    
    self.dataFileNamesList = [ ] #Stores the file names for the data sets
    self.classificationResults = [ ] #Stores the results of the classifications
    
    
  #Called by the user to begin building a confusion matrix  
  def Build( self ):  
    
    self.LoadDataSetsList( )
    self.SetupClassificationResults( )
    self.BuildConfusionMatrix( )
    self.PrintConfusionMatrix( )
    
    
  #Loads the names of the dataSets  
  def LoadDataSetsList( self ):
    
    self.dataFileNamesList = glob.glob( "classificationResults\*.dat" )
    
  def BuildConfusionMatrix( self ):
    
    for item in self.classificationResults:
    
      i = 0
      j = 0
      
      print item.dataSetTrueAnnotation
      print item.dataSetClassifiedAnnotation
      
      if( item.dataSetTrueAnnotation == "runn" ):
      
        i = 0
        
      elif ( item.dataSetTrueAnnotation == "bike" ):
      
        i = 1
        
      else :
          
        i = 2
        
      if( item.dataSetClassifiedAnnotation == "runn" ):
      
        j = 0
        
      elif ( item.dataSetClassifiedAnnotation == "bike" ):
      
        j = 1
        
      else :
          
        j = 2
       
      print "i = ", i, " j = ", j 
      self.confusionMatrix[ i ][ j ] += 1
      
  
  def PrintConfusionMatrix( self ):
  
    print "  R B S"
    
    modelChar = ""
    
    
    for i in xrange( 0, 3 ):
    
      if( i == 0 ):
        modelChar = "R"
      elif( i == 1 ):
        modelChar = "B"
      else:
        modelChar = "S"
    
      print modelChar, self.confusionMatrix[ i ] [ 0 ], self.confusionMatrix[ i ] [ 1 ], self.confusionMatrix[ i ] [ 2 ]
    
    
   
    
    print "printing matrix"
    
  def SetupClassificationResults( self ):
  
    #Temporarily holds the classification scores for use with classification
    runningScore = 0.0
    bikingScore = 0.0
    walkingScore = 0.0
    
    classificationFileIndex = 0;
    
    for item in self.dataFileNamesList:
    
      runningScore = 0.0
      bikingScore = 0.0
      standingScore = 0.0

      classificationResultFile = open( item )
        
      #self.classificationResults[ classificationFileIndex ] = new ClassificationResult( )
      classificationResult = ClassificationResult( )
      
      testingString = item[ len( "classificationResults" ) + 1: len( "classificationResults" ) + 5 ]
      
      if( testingString == "runn" ):
        
        classificationResult.dataSetTrueAnnotation = "runn"
        
      elif( testingString == "bike" ):
      
        classificationResult.dataSetTrueAnnotation = "bike"
        
      else:
      
        classificationResult.dataSetTrueAnnotation = "stnd"
      
      for line in classificationResultFile.readlines( ):
      
        scores = line.split( " ", 3 )
        
        runningScore = runningScore + float( scores[ 0 ] )
        bikingScore = bikingScore + float( scores[ 1 ] )
        standingScore = standingScore + float( scores[ 2 ] )
      
      print "fileName = ", item
      print "detectedAnnotation = ", classificationResult.dataSetTrueAnnotation
      
      print runningScore
      print bikingScore
      print standingScore
      print " "
      
      if( runningScore >= bikingScore and runningScore >= standingScore ):
      
        classificationResult.dataSetClassifiedAnnotation = "runn"
        
      elif( bikingScore >= runningScore and bikingScore >= standingScore):
      
        classificationResult.dataSetClassifiedAnnotation = "bike"
        
      else:
      
        classificationResult.dataSetClassifiedAnnotation = "stnd"
        
      print classificationResult.dataSetClassifiedAnnotation
        
      self.classificationResults.append( classificationResult )
      
      classificationFileIndex += 1
    
def main( ):

  print "inside main"
  
  confusionMatrixBuilder = ConfusionMatrix( )
  
  confusionMatrixBuilder.Build( )
  
main( )