from .Actor import Actor

class WriteFile( Actor ):
    def on_start( self, content, ext = "", stem = "src" ) -> None:
        filename = self.make_output_filename( ext, stem, [ "src" ] )
        with open( filename, 'w' ) as fout:
            fout.write( content )

        self.on_end( filename )
