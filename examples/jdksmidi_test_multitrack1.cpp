/*
 *  libjdksmidi-2004 C++ Class Library for MIDI
 *
 *  Copyright (C) 2004  J.D. Koftinoff Software, Ltd.
 *  www.jdkoftinoff.com
 *  jeffk@jdkoftinoff.com
 *
 *  *** RELEASED UNDER THE GNU GENERAL PUBLIC LICENSE (GPL) April 27, 2004 ***
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
//
// Copyright (C) 2010 V.R.Madgazin
// www.vmgames.com vrm@vmgames.com
//

#include "jdksmidi/world.h"
#include "jdksmidi/track.h"
#include "jdksmidi/multitrack.h"
#include "jdksmidi/filereadmultitrack.h"
#include "jdksmidi/fileread.h"
#include "jdksmidi/fileshow.h"

using namespace jdksmidi;


void DumpMIDITimedBigMessage ( const MIDITimedBigMessage *msg )
{
    if ( msg )
    {
        char msgbuf[1024];
        fprintf ( stdout, "%8ld : %s\n", msg->GetTime(), msg->MsgToText ( msgbuf ) );

        if ( msg->IsSystemExclusive() )
        {
            fprintf ( stdout, "\tSYSEX length: %d\n", msg->GetSysEx()->GetLength() );
        }
    }
}

void DumpMIDITrack ( MIDITrack *t )
{
    MIDITimedBigMessage *msg;

    for ( int i = 0; i < t->GetNumEvents(); ++i )
    {
        msg = t->GetEventAddress ( i );
        DumpMIDITimedBigMessage ( msg );
    }
}

void DumpAllTracks ( MIDIMultiTrack *mlt )
{
    for ( int i = 0; i < mlt->GetNumTracks(); ++i )
    {
        if ( mlt->GetTrack ( i )->GetNumEvents() > 0 )
        {
            fprintf ( stdout, "DUMP OF TRACK #%2d:\n", i );
            DumpMIDITrack ( mlt->GetTrack ( i ) );
            fprintf ( stdout, "\n" );
        }
    }
}


void DumpMIDIMultiTrack ( MIDIMultiTrack *mlt )
{
    MIDIMultiTrackIterator i ( mlt );
    const MIDITimedBigMessage *msg;
    i.GoToTime ( 0 );

    do
    {
        int trk_num;

        if ( i.GetCurEvent ( &trk_num, &msg ) )
        {
            fprintf ( stdout, "#%2d - ", trk_num );
            DumpMIDITimedBigMessage ( msg );
        }
    }
    while ( i.GoToNextEvent() );
}



int main ( int argc, char **argv )
{
    if ( argc > 1 )
    {
        MIDIFileReadStreamFile rs ( argv[1] );
        MIDIMultiTrack tracks;
        MIDIFileReadMultiTrack track_loader ( &tracks );
        MIDIFileRead reader ( &rs, &track_loader );
        reader.Parse();
        DumpMIDIMultiTrack ( &tracks );
    }

    return 0;
}
