import os
import glob

numberOfDuplicateClassModels = raw_input( "How many duplicate models are there if none put 0: " )

class ClassificationResult:

  def __init__( self ):
  
    self.similarityScore = 0
    self.dataSetTrueAnnotation = ""
    self.dataSetClassifiedAnnotation = ""
    
#Possible classifications are running, biking, and walking (RBS). The results of this algorithm are very
#dependent on the format of the classification results. It is assuming they are stored as running results, biking results, and walking results.    
    
class ConfusionMatrix:

  def __init__( self ):

    self.confusionMatrix = [ [ ] ] #Stores the results of each classification
    
    self.dataFileNamesList = [ ] #Stores the file names for the data sets
    self.classificationResults = [ ] #Stores the results of the classifications
    self.annotationList = [ ]
    self.similarityScoreList = [ ]
    
  #Called by the user to begin building a confusion matrix  
  def Build( self ):  
    
    self.LoadDataSetsList( )
    self.SetupClassificationResults( )
    self.BuildConfusionMatrix( )
    self.PrintConfusionMatrix( )
    
  #Loads the names of the dataSets, number of possible annotations and a list of the annotations
  def LoadDataSetsList( self ):
    
    #Returns a list containing every file inside the classificationResults folder
    self.dataFileNamesList = glob.glob( "classificationResults\*.dat" )
    
    for item in self.dataFileNamesList:
    
      #Extracts the first 4 characters, minus the file path, which is used to annotate the data
      testingString = item[ len( "classificationResults" ) + 1: len( "classificationResults" ) + 5 ]
      
      #print testingString
      
      if testingString not in self.annotationList:
      
        self.similarityScoreList.append( 0 )
        self.annotationList.append( testingString )
        
        
      #print self.annotationList
      
    self.dataFileNamesList = glob.glob( "classificationResults\*.dat" )
    
    #Creates a matrix of number of annotations by number of annotations i.e 3X3 or 5X5
    self.confusionMatrix = [ [ 0 for col in range( len( self.annotationList ) ) ] for row in range( len( self.annotationList ) ) ]
    
    
  def ResetScoresList( self ):
  
    self.similarityScoreList = [ 0.0 ] * len( self.annotationList )
    
  def BuildConfusionMatrix( self ):
    
    for item in self.classificationResults:
    
      iIndex = 0
      jIndex = 0
      
      print item.dataSetTrueAnnotation
      print item.dataSetClassifiedAnnotation
      #raw_input()
      #Replace with a for loop that loops through the possible annotations
      #When one matches assign the integer
      
      for i in xrange( 0, len( self.annotationList ) ):
      
        if( item.dataSetTrueAnnotation == self.annotationList[ i ] ):
        
          #print "inside i if"
        
          iIndex = i
      
      for j in xrange( 0, len( self.annotationList ) ):
      
        if( item.dataSetClassifiedAnnotation == self.annotationList[ j ] ):
        
          #print "inside j if"
        
          jIndex = j
      
      '''
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
      '''
      
      print "i = ", iIndex, " j = ", jIndex 
      self.confusionMatrix[ iIndex ][ jIndex ] += 1
      
  
  def PrintConfusionMatrix( self ):
  
    #print "  R B S"
    
    modelChar = ""
    
    
    print " ",
    
    for i in xrange( 0, len( self.annotationList ) ):
    
      print "", self.annotationList[ i ][ 0 : 3 ],
    
    print ""
    
    for i in xrange( 0, len( self.annotationList ) ): #replade 3 with the number of possible annotations
 
      #Not sure what to do yet
      '''
      if( i == 0 ):
        modelChar = "R"
      elif( i == 1 ):
        modelChar = "B"
      else:
        modelChar = "S"
      '''
      
      print self.annotationList[ i ][ 0 : 3 ],
      
      for j in xrange( 0, len( self.annotationList ) ): #Replace 3 with the number of possible annotations
      
      #print modelChar, self.confusionMatrix[ i ] [ 0 ], self.confusionMatrix[ i ] [ 1 ], self.confusionMatrix[ i ] [ 2 ]
        print self.confusionMatrix[ i ][ j ], "  ",
        
      #print "|",
      print ""
    
   
    print self.annotationList
    
    print "printing matrix"
    
  def SetupClassificationResults( self ):
  
    #Replace with a simple list of scores where each index corresponds to an index in the annotationList list
    '''
    #Temporarily holds the classification scores for use with classification
    runningScore = 0.0
    bikingScore = 0.0
    walkingScore = 0.0
    '''
    
    classificationFileIndex = 0;
    
    scoresIndex = 0
    
    for item in self.dataFileNamesList:
    
      
      
      '''    
      runningScore = 0.0
      bikingScore = 0.0
      standingScore = 0.0
      '''
      
      self.ResetScoresList( )
      
      classificationResultFile = open( item )
        
      #self.classificationResults[ classificationFileIndex ] = new ClassificationResult( )
      classificationResult = ClassificationResult( )
      
      #Extracts the 4 character identifier for the file
      testingString = item[ len( "classificationResults" ) + 1: len( "classificationResults" ) + 5 ]
      
      #Replace with classificationResult.dataSetTrueAnnotation = testingString
      '''
      if( testingString == "runn" ):
        
        classificationResult.dataSetTrueAnnotation = "runn"
        
      elif( testingString == "bike" ):
      
        classificationResult.dataSetTrueAnnotation = "bike"
        
      else:
      
        classificationResult.dataSetTrueAnnotation = "stnd"
      '''
      
      classificationResult.dataSetTrueAnnotation = testingString
      
      for line in classificationResultFile.readlines( ):
      
        scoresIndex = 0
      
        scores = line.split( " ", len( self.annotationList ) ) #Replace 3 with the number of possibilities
       
        #print scores
       
        #print len( scores )
       
        for item in scores:
        
          try:
          
            self.similarityScoreList[ scoresIndex ] = self.similarityScoreList[ scoresIndex ] + float( scores[ scoresIndex ] )
          
          except:
          
            print "error"
            print scoresIndex
            print len( self.annotationList )
            print len( self.similarityScoreList )
            self.similarityScoreList[ scoresIndex ] = self.similarityScoreList[ scoresIndex ] + 0
            
          
          #print scoresIndex
         # print len( self.annotationList )
         # print len( self.similarityScoreList )
          scoresIndex += 1
          #print scoresIndex
        
        '''
        runningScore = runningScore + float( scores[ 0 ] )
        bikingScore = bikingScore + float( scores[ 1 ] )
        standingScore = standingScore + float( scores[ 2 ] )
        '''
        
      print "fileName = ", item
      print "detectedAnnotation = ", classificationResult.dataSetTrueAnnotation
      
      '''
      print runningScore
      print bikingScore
      print standingScore
      print " "
      '''
      
      highestScoreIndex = 0
      highestScore = 0
      
      for i in xrange( 0, len( self.annotationList ) ):
      
        if self.similarityScoreList[ i ] > highestScore:
        
          highestScoreIndex = i
          highestScore = self.similarityScoreList[ i ]
          
      
      classificationResult.dataSetClassifiedAnnotation = self.annotationList[ highestScoreIndex ]
      
      #Replace with a simple for and if statement
      '''
      if( runningScore >= bikingScore and runningScore >= standingScore ):
      
        classificationResult.dataSetClassifiedAnnotation = "runn"
        
      elif( bikingScore >= runningScore and bikingScore >= standingScore):
      
        classificationResult.dataSetClassifiedAnnotation = "bike"
        
      else:
      
        classificationResult.dataSetClassifiedAnnotation = "stnd"
      '''
      
      print classificationResult.dataSetClassifiedAnnotation
        
      self.classificationResults.append( classificationResult )
      
      classificationFileIndex += 1
    
def main( ):

  print "inside main"
  
  confusionMatrixBuilder = ConfusionMatrix( )
  
  confusionMatrixBuilder.Build( )
  
main( )