A Simple MUD Client v1.01
-------------------------

WHAT IT IS

This is a small telnet client, that can be used to connect to a MUD. It
understands ANSI colour and it has a 500 line scrollback buffer. You can
scroll through it with PageUp/PageDown, or with the mouse.

Unlike the other ANSI applets I've seen, it also has a separate input line,
so the text you type doesn't get mixed up with incoming text from the MUD.
The command line has a history buffer, and remembers the last 32 commands
entered. Use the up and down arrows to scroll through the remembered
commands.

Note that, to make the most out of this limited buffer, commands
entered more than once are only stored once in the history buffer. E.g.,
if you enter the sequence of commands "inventory", "look", "consider all",
and again "look", the first instance of "look" will be erased, to conserve
buffer space. I.e., the history buffer will now look like: "inventory",
"consider all" and "look". To put it otherwise, it tries to remember up to
32 _different_ commands, which hopefully should be more useful than if 20
of them were duplicates.

Also, being specially designed for MUD's, the keypad acts as a direction
pad. E.g., you can press '7' on the keypad and have 'nw' sent to the MUD.
It really makes walking around faster and easier. (You must have NumLock ON
for this to work.)

The function keys also produce some useful commands. (Though if you're not
on Discworld or a derivative thereof, they may not match the commands
understood by the MUD, and thus will be less useful. In a future release,
I might make the program load the commands from a text file.)

Due to popular request ;) it now does wrap incoming text, and it does it
by breaking the line at a space character. It turned out that you can't
reliably expect the MUD to always do the wrapping for you. Note that, due
to speed reasons, it only wraps incoming text, not lines of text already
received. I.e., if you resize the screen or change the font size, the
already received text isn't re-formatted to the new width. (But any text
you receive from now on, obviously is.)

WHAT IT ISN'T

Briefly, it isn't a general purpose terminal emulation. It understands
the ANSI colour codes, and the ANSI bright and reverse video commands.
But that's all. It does NOT understand the cursor positioning commands,
nor underline, nor italic, nor the rest of the ANSI standard. (At least
not for now.)

I.e., it should be ideal for connecting to most MUD's, but not for playing
a telnet version of space invaders, because it lacks cursor positioning.

Also, it doesn't have local scripting, aliases, timers or triggers. At
least, not for now. Those would significantly inflate the size and
number of classes downloaded, when used as an applet. I.e., it would make
the applet load a whole lot slower. If you really need those, grab a copy
of zMud or gMud instead.

THE BUTTONS

Disconnect -- Disconnects you from the MUD. Note that this will make you
net-dead, instead of properly logging you out, so it's generally bad
practice.

Connect -- Connects you to the MUD. If you were connected already, it
first disconnects you, and then connects again.

Large Font -- Use 14 pixel bold Courier for a font. You'll appreciate this
if you're using very high resolution, like I do :) If your MUD auto-detects
screen size, but it is NOT fully compliant with RFC 1073, you may need to
reconnect to force it to notice the new screen size.

Small Font -- Use 12 pixel plain Courier for a font. Mainly there for those
of you with 640x480 screens. If your MUD auto-detects screen size, but it is
NOT fully compliant with RFC 1073, you may need to reconnect to force it to
notice the new screen size.

Detach -- If it's used as an applet, detaches it from the html page and
into a separate frame. I.e., now you can resize it freely, to suit your
preferences. This button will only be visible if it's run as an applet, and
it isn't detached already.

Attach -- Nukes the frame, and attaches the client back onto the html
page. This button will only be visible if it's run as an applet, and
it is currently detached.

HINTS

On Discworld or derivatives, use 'term network'. This will enable negotiating
window size on the fly. I.e., when you change the window size or font size,
Discworld will automatically adjust its text wrapping and pausing.

COMPATIBILITY CONSIDERATIONS

The applet uses the 1.1 AWT event model, as opposed to the more simplistic
1.02. I.e., you'll need a browser that does understand Java 1.1 to use it
as an applet, or a 1.1 or 1.2 compatible JDK or JRE to run it as a standalone
application.

It has been extensively tested as an applet with Netscape 4.51 and 4.6, and
Internet Explorer 5.0. As an application it has been tested with Sun's JDK
1.2.1 with the HotSpot JVM, IBM's 1.1.7 JRE, and Microsoft's Virtual Machine
build 3167 and 3177.

If you have Netscape Explorer 4.05 or earlier, it has a problem with
understanding the 1.1 AWT event model. Allegedly, there is a patch for that.
Or you can just get a newer version of Netscape.

Also, I would recommend NOT using Internet Explorer or the Microsoft JVM to
run it. They are buggy and slightly incompatible with the standard. Note that
I'm not preaching anti-Microsoft theory, but it's based on actual testing.
E.g., under my current Microsoft JVM, it doesn't seem to notice the PageUp and
PageDown keys, so you're stuck with using a mouse to scroll back. Also, it
doesn't seem to allow using ALT and the keypad to enter extended characters.
Unless you enjoy wanton incompatibilities and headaches, avoid Microsoft's
Java implementations. It's that simple.

USING IT

The client can be used either as an applet, or as a standalone application.
The main advantage as an application is that you can run it locally, and
connect to any site you want to.

To use it as an application, you must, (A) have the classes on your local
machine, and (B) have a Java Development Kit (JDK) or a Java Runtime
Environment (JRE) installed and accessible. Your JDK or JRE must implement
at least the Java 1.1 specs. Try one of the following, and see if it works.

    jre -cp . ANSI_Client atuin.icepack.com 23

or

    java ANSI_Client atuin.icepack.com 23

As you probably guessed, the first parameter is the host name or IP address,
and the second is the port number.

To use it as an applet, include the following on one of your HTML pages:

    <applet code="ANSI_Client.class" width=750 height=500 align=center>
    <param name="host" value="atuin.icepack.com">
    <param name="port" value="23">
    <param name="buttons" value="Y">
    Sorry, your browser doesn't seem able to run Java applets.
    Try using a normal telnet client instead.
    </applet>

You may change the "buttons" parameter to

    <param name="buttons" value="N">

to skip the two buttons at the top. This should give you more lines of
text instead.

Note that due to Java's security restrictions, when used as an applet, it
can only connect to the server it was loaded from. I.e., you must either
have the web server on the same machine as the MUD, or have a sort of
telnet proxy on your web server machine, that accepts connections from the
client, and redirects them to the actual MUD machine.

VERSION HISTORY

1.0

  Initial release.

1.01

  Added text wrapping.

  Added small/large font option.

  Added informing the server of changes in screen size, whenever they occur,
  if it has requested NAWS (Negotiate About Screen Size.) I.e., _if_ the
  server is fully compliant with RFC 1073, it should addapt its text wrapping
  and pausing to your new screen size, on the fly. (The client's own wrapping
  is of course automatically addapted anyway, since it's local, and doesn't
  rely on any telnet features from the server.)

  Fixed handling of character 255 (telnet IAC code). Now it doubles IAC's it
  sends as part of the outgoing text, and correctly handles incoming
  doubled IAC's. Previously, it discarded incoming doubled IAC's. (Unlikely
  to be a problem, unless you're using a language which uses character 255.
  In the normal "Latin 1" encoding, a.k.a. "Western", it's an umlaut-y.)
  (Note that some MUD servers have problems themselves with handling IAC's
  as part of the text, so you may well still have problems using that
  character.)

  Fixed response to unsupported WILL option offers from the server. Now it
  correctly responds DON'T, instead of WON'T. (Not that I noticed any problem
  with the previous response, but just to fully comply with RFC 854 and 855.)

  Fixed terminal detection, so now repeatedly connecting won't cycle through
  "ANSI", "VT100" and "UNKNOWN". (I.e., it starts from the top of the list
  for each IAC DO TERMINAL-TYPE, as per RFC 1091.)

  Fixed NAWS reporting, for the case your screen resolution is large enough
  to have 128 or more rows or columns. (For some reason, Discworld seems
  to be freaked out by more than 127 rows or columns, and resets them to 63.
  So now I limit the reporting to a maximum of 127 each, to avoid it.)

1.02

  Added detaching and attaching as an applet.

Have fun,
Cristian Golumbovici
