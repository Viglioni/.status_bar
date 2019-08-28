#!/bin/bash

# Script based on https://gist.github.com/wandernauta/6800547
################################################
# Copyright (C) 2013 Wander Nauta
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software, to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to permit
# persons to whom the Software is furnished to do so, subject to the following
# conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# The software is provided "as is", without warranty of any kind, express or
# implied, including but not limited to the warranties of merchantability,
# fitness for a particular purpose and noninfringement. In no event shall the
# authors or copyright holders be liable for any claim, damages or other
# liability, whether in an action of contract, tort or otherwise, arising from,
# out of or in connection with the software or the use or other dealings in the
# software.
################################################

# Copyright (C) 2019 Laura Viglioni

# Requires
# require dbus-send
# require grep
# require sed
# require cut
# require tr

SP_DEST="org.mpris.MediaPlayer2.spotify"
SP_PATH="/org/mpris/MediaPlayer2"
SP_MEMB="org.mpris.MediaPlayer2.Player"

getArtist (){
  
    dbus-send                                                                   \
	      --print-reply                                  `# We need the reply.`       \
	      --dest=$SP_DEST                                                             \
	      $SP_PATH                                                                    \
	      org.freedesktop.DBus.Properties.Get                                         \
	      string:"$SP_MEMB" string:'Metadata'                                         \
	      | grep -Ev "^method"                           `# Ignore the first line.`   \
	      | grep -Eo '("(.*)")|(\b[0-9][a-zA-Z0-9.]*\b)' `# Filter interesting fiels.`\
	      | sed -E '2~2 a|'                              `# Mark odd fields.`         \
	      | tr -d '\n'                                   `# Remove all newlines.`     \
	      | sed -E 's/\|/\n/g'                           `# Restore newlines.`        \
	      | sed -E 's/(xesam:)|(mpris:)//'               `# Remove ns prefixes.`      \
	      | sed -E 's/^"//'                              `# Strip leading...`         \
	      | sed -E 's/"$//'                              `# ...and trailing quotes.`  \
	      | sed -E 's/"+/|/'                             `# Regard "" as seperator.`  \
	      | sed -E 's/ +/ /g'                            `# Merge consecutive spaces.` \
	      | grep artist | sed 's/artist|//'
		    
}

getArtist


