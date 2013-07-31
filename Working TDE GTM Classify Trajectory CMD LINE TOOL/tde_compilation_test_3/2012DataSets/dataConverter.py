#A simple script to convert the data from last year into a format that Jordan Frank's code can use.
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
    
    self.dataFileOutName = "convertedDataSets.dat"
    self.fileOut = open( self.dataFileOutName, "w" )
  def StartTheConversion( self ):  
    
    self.SetupFileNamePaths( )
    self.GetDataFiles( )
    
  def SetupFileNamePaths( self ):

    print "setting up"
    
    self.directoryNames = [ o for o in os.listdir( "." ) if os.path.isdir( o ) ] #Source for the looping code: #http://stackoverflow.com/questions/973473/getting-a-list-of-all-subdirectories-in-the-current-directory
    
    for item in self.directoryNames:
    
      print item
      print os.listdir( item )
    
      self.childDirectories.append( os.listdir( item ) )
    
    #print self.childDirectories
    
    f = open( self.ADLFile )
    
    self.fileAnnotations = [ "LYBE", "SIBE", "SCHH", "SSOO", "SAII", "WAFF", "JOFF", "WABB", "BEXX", "BEPP", "SQDD"  ]
    
    #print self.fileAnnotations
    #print self.directoryNames
    #print self.childDirectories
   
  def GetDataFiles( self ):
 
     annotationsIndex = 0
     directoriesIndex = 1
     
     counter = 0
      
     #print self.childDirectories[ 1 ]
     raw_input()
      
     for directory in self.directoryNames:
 
        print directory
        print "length = " + str( len( self.childDirectories[ directoriesIndex ] ) )
        counter = 0
 
        
        self.fileName = self.fileAnnotations[ annotationsIndex ] + str( counter ) + "-.dat"

          
        #current_directory = directory + "\\" + self.childDirectories[ directoriesIndex ] [ i ]
      
        self.ConvertData( directory, self.childDirectories[ directoriesIndex ] )
      
       # counter +=1 
        
        print "before increment"
        annotationsIndex += 1
        directoriesIndex += 1
  
     print "getting files"
    
  def ConvertData( self, parentDirectory, directories ):

    #print directory
  
    print "converting data"
  
    fileOut = open( "..\dataSets\\" + self.fileName, "a" )
    
    for directory in directories:
    
      print directory
    
      f = open( parentDirectory + "\\" + directory + "\RSS_Service1.txt", "r" )
    
      for line in f.readlines( ):
    
        lineSplit = line.split( " " )
      
        if( lineSplit[ 0 ] == "RSS:" ):
      
          fileOut.write( lineSplit[ 1 ] + "\n" ) 
          
    #fileOut = open( self.dataFileOutName, "a" )
    self.fileOut.write( self.fileName[ 0 : len( self.fileName ) - 4 ] + "\n" )

        
def main( ):

  dataConverter = DataConverter( )
  
  dataConverter.StartTheConversion( )

main( )