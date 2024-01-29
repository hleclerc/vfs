from .Command import Command
from .Actor import Actor
from .Link import Link

class LinkAndLaunch( Actor ):
    def on_start( self, sources: list[ str ] ):
        self.launch( self.on_link, Link(), 'exe', sources )

    def on_link( self, **kwargs ):
        args = []
        if self.options[ "valgrind" ]:
            args.append( "valgrind" )
        args.append( kwargs[ "out_name" ] )

        self.launch( self.on_end, Command(), args, check_return_code = False )

    def is_idempotent( self ):
        return False
