using Netch.Interfaces;
using Netch.Models;

namespace Netch.Servers;

public class HttpController : IServerController
{
    public string Name { get; } = "HTTP";

    public ushort? Socks5LocalPort { get; set; }

    public string? LocalAddress { get; set; }

    public Task<Socks5Server> StartAsync(Server s)
    {
        var server = (HttpServer)s;
        return Task.FromResult<Socks5Server>(server);
    }

    public Task StopAsync()
    {
        return Task.CompletedTask;
    }
}
