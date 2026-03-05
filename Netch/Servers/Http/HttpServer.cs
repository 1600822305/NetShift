using Netch.Models;

namespace Netch.Servers;

public class HttpServer : Socks5Server
{
    public override string Type { get; } = "HTTP";

    public HttpServer()
    {
    }

    public HttpServer(string hostname, ushort port) : base(hostname, port)
    {
    }

    public HttpServer(string hostname, ushort port, string username, string password) : base(hostname, port, username, password)
    {
    }
}
