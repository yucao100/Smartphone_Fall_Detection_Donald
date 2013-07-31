#A simple script to convert the data from last year into a format that Jordan Frank's code can use.
#Make sure the ADL's.txt file from last semester is included in the same directory as this script
import os

class DataConverter:

  def __init__( self ):
  
    print "inside init"
    
    #These match exactly The first item in fileNames is equivalent to the list of child directories
    #within that directory, and the fileAnnotations list is synced with the fileNames list
    
    self.directoryNames = [  ] #Holds the original folder names
    self.childDirectories = [ [ ] ] #Holds the child directories within that file
    self.fileAnnotations = [ ]
    
    self.ADLFile = "ADLs.txt"
    
    self.fileName = ""
    
  def StartTheConversion( self ):  
    
    self.SetupFileNamePaths( )
    self.GetDataFiles( )
    
  def SetupFileNamePaths( self ):

    print "setting up"
    
    self.directoryNames = [ o for o in os.listdir( "." ) if os.path.isdir( o ) ] #Source for the looping code: #http://stackoverflow.com/questions/973473/getting-a-list-of-all-subdirectories-in-the-current-directory
    
    for item in self.directoryNames:
    
      self.childDirectories.append( os.listdir( item ) )
    
    f = open( self.ADLFile )
    
    for item in f.readlines( ):
    
      self.fileAnnotations.append( item.split( "\t" )[ 1 ] )
    
    print self.fileAnnotations
    print self.directoryNames
    print self.childDirectories
   
  def GetDataFiles( self ):
 
     annotationsIndex = 0
     counter = 0
    
     for directory in self.directoryNames:
 
      for childDirectory in self.childDirectories:
      
        self.fileName = self.fileAnnotations[ annotationsIndex ] + str( counter ) + "-.dat"
      
        current_directory = directory, "/", childDirectory
      
        convertData( current_directory )
      
        counter +=1 
        
      annotationsIndex += 1
  
    #print "getting files"
    
  def ConvertData( self, directory ):

    f = open( directory + "RSS_Service1.txt", "r" )
    
    fileOut = open( "../../" + self.fileName, "w" )
    
    for line in f.readlines( ):
    
      lineSplit = line.split( " " )
      
      if( lineSplit[ 0 ] == "RSS:" ):
      
        fileOut.write( lineSplit[ 1 ] + "\n" ) 

        
def main( ):

  dataConverter = DataConverter( )
  
  dataConverter.StartTheConversion( )

main( )