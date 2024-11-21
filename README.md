<h1>42ft_irc</h1>

<h2>Overview</h2>
<p><strong>42ft_irc</strong> is a project focused on building your own IRC server. The aim is to implement an IRC (Internet Relay Chat) server based on solid internet protocol standards, enabling clients to connect, communicate, and interact with your server. You will use an actual IRC client to connect and test its functionality.</p>

<h2>Features</h2>
    <ul>
    <li>Develop a fully functional IRC server.</li>
    <li>Adhere to the core IRC protocol standards.</li>
        <li>Enable multiple clients to connect and interact.</li>
        <li>Test the server using existing IRC clients.</li>
    </ul>

<h2>Learning Goals</h2>
    <ul>
    <li>Understand and implement communication based on standard protocols.</li>
    <li>Explore network programming, including sockets and message handling.</li>
        <li>Learn about the fundamentals of the IRC protocol.</li>
        <li>Strengthen debugging and problem-solving skills in a multi-client environment.</li>
    </ul>

<h2>How to Use</h2>
    <ol>
    <li><strong>Clone the repository</strong>: 
        <pre>
git clone https://github.com/<username>/42ft_irc.git
cd 42ft_irc
            </pre>
        </li>
        <li><strong>Build the project</strong>: 
            <pre>
make
            </pre>
        </li>
        <li><strong>Run the server</strong>: 
            <pre>
./ircserver &lt;port&gt; &lt;password&gt;
            </pre>
        </li>
        <li><strong>Connect using an IRC client</strong>: 
            <p>Use an IRC client of your choice (e.g., HexChat, mIRC, irssi) and connect to your server:</p>
            <pre>
/server &lt;IP address&gt; &lt;port&gt; -p &lt;password&gt;
            </pre>
        </li>
    </ol>

<h2>Requirements</h2>
    <ul>
    <li><strong>Language</strong>: C++</li>
    <li><strong>Standards</strong>: C++98</li>
        <li><strong>Libraries</strong>: None (no external libraries are allowed)</li>
    </ul>

<h2>Protocol References</h2>
    <ul>
    <li><a href="https://datatracker.ietf.org/doc/html/rfc1459">RFC 1459: Internet Relay Chat Protocol</a></li>
    <li><a href="https://datatracker.ietf.org/doc/html/rfc2812">RFC 2812: IRC Client Protocol</a></li>
    </ul>

<h2>Credits</h2>
    <p>This project is part of the 42 Network curriculum. Special thanks to <a href="https://github.com/davimeireles"/a>Davi Meireles</p>
</body>
</html>
