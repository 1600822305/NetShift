using Netch.Models.Modes;

namespace Netch.Models;

public class Profile
{
    public int Index { get; set; }

    public string ModeRemark { get; set; }

    public string ProfileName { get; set; }

    public string ServerRemark { get; set; }

    public string ServerType { get; set; }

    public Profile(Server server, Mode mode, string name, int index)
    {
        ServerRemark = server.Remark;
        ServerType = server.Type;
        ModeRemark = mode.i18NRemark;
        ProfileName = name;
        Index = index;
    }

    public Profile()
    {
        ServerRemark = string.Empty;
        ServerType = string.Empty;
        ModeRemark = string.Empty;
        ProfileName = string.Empty;
        Index = 0;
    }
}