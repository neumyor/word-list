# -- encoding:utf-8 --

import ctypes
import os
import subprocess
import sys

import wx


class MyFrame(wx.Frame):
    def __init__(self, parent, id, title, pos, size):
        wx.Frame.__init__(self, parent, id, title, pos, size)
        self.panel = wx.Panel(self)  # 创建一个panel

        # 文本输出框 （pos是输出框位置，size是大小，可自行修改）
        self.output_box = wx.TextCtrl(self.panel, pos=(500, 25), size=(460, 510),
                                      style=wx.TE_MULTILINE | wx.TE_READONLY | wx.TE_RICH | wx.EXPAND)

        self.is_h = False
        self.is_t = False
        self.is_r = False

        self.n_file_path = None
        self.w_file_path = None
        self.m_file_path = None
        self.c_file_path = None

        baseline = 20
        gap = 60
        lap_1 = 50
        gap_2 = 30

        self.use_file = True
        self.file_path = None
        self.process_type = 'n'

        # 选择输入类型
        self.select_input_type_box = wx.RadioBox(self.panel, -1, "选择输入类型", (20, baseline), (150, baseline),
                                                 ["使用文件作为输入", "使用文本作为输入"])
        self.select_input_type_box.Bind(wx.EVT_RADIOBOX, self.select_input_type)

        self.file_picker = wx.FilePickerCtrl(self.panel, -1, "选择输入文件", pos=(50, baseline + 60),
                                             style=wx.FLP_USE_TEXTCTRL,
                                             size=(300, 30))
        self.file_picker.Bind(wx.EVT_FILEPICKER_CHANGED, self.select_file)

        self.text_input = wx.TextCtrl(self.panel, -1, "请在这里输入你的文本", pos=(50, baseline + 60), size=(400, 40),
                                      style=wx.TE_MULTILINE)
        self.text_input.Hide()

        # 选择处理方式
        self.select_process_type_box = wx.RadioBox(self.panel, -1, "选择处理方式",
                                                   pos=(50, baseline + lap_1 + gap),
                                                   style=wx.RA_SPECIFY_COLS,
                                                   majorDimension=1,
                                                   choices=["统计文本中共有多少条单词链（包括嵌套单词链）并输出",
                                                            "计算最多单词数量的英语单词链并输出",
                                                            "计算首字母不相同的以单词数计最长单词链并输出",
                                                            "计算字母最多的单词链并输出"])

        self.select_process_type_box.Bind(wx.EVT_RADIOBOX, self.select_process_type)
        # 指定其他元素

        self.is_h_check = wx.CheckBox(self.panel, -1, "指定单词链的首字母", pos=(50, baseline + lap_1 + 2 * gap + 3 * gap_2))
        self.first_char_input = wx.TextCtrl(self.panel, -1, "", pos=(50, baseline + lap_1 + 2 * gap + 3 * gap_2 + 30),
                                            size=(20, 20))
        self.first_char_input.SetMaxLength(1)
        self.first_char_input.Hide()

        self.is_t_check = wx.CheckBox(self.panel, -1, "指定单词链的尾字母", pos=(50, baseline + lap_1 + 3 * gap + 3 * gap_2))
        self.last_char_input = wx.TextCtrl(self.panel, -1, "", pos=(50, baseline + lap_1 + 3 * gap + 3 * gap_2 + 30),
                                           size=(20, 20))
        self.last_char_input.SetMaxLength(1)
        self.last_char_input.Hide()

        self.is_r_check = wx.CheckBox(self.panel, -1, "允许文本隐含单词环", pos=(50, baseline + lap_1 + 4 * gap + 3 * gap_2))

        self.is_h_check.Bind(wx.EVT_CHECKBOX, self.on_h_check)
        self.is_t_check.Bind(wx.EVT_CHECKBOX, self.on_t_check)
        self.is_r_check.Bind(wx.EVT_CHECKBOX, self.on_r_check)

        self.start_button = wx.Button(self.panel, -1, "开始", pos=(100, 500))
        self.start_button.Bind(wx.EVT_BUTTON, self.start)
        self.download_button = wx.Button(self.panel, -1, "下载输出到指定文件", pos=(200, 500))
        self.download_button.Bind(wx.EVT_BUTTON, self.download)
        self.clear_output = wx.Button(self.panel, -1, "清空输出栏", pos=(400, 500))
        self.clear_output.SetBackgroundColour("#EE6363")
        self.clear_output.Bind(wx.EVT_BUTTON, lambda evt: self.output_box.Clear())

        # 命令执行方法

    def start(self, evt):
        print("start execution")
        cmd = 'Word-Chain.exe '
        if self.is_h:
            cmd += f"-h {self.first_char_input.GetValue()} "
        if self.is_t:
            cmd += f"-t {self.last_char_input.GetValue()} "
        if self.is_r:
            cmd += f"-r "

        cmd += f"-{self.process_type} "

        if self.use_file:
            cmd += f"{self.file_path}"
        else:
            with open("temp.txt", "w") as f:
                f.write(self.text_input.GetValue())
            cmd += "temp.txt"

        error_msg = ""

        if self.use_file and (self.file_path is None or not os.path.exists(self.file_path)):
            error_msg += "未选择保存路径 或 保存路径无效\n"

        if not self.use_file:
            for ch in self.text_input.GetValue():
                if u'\u4e00' <= ch <= u'\u9fff':
                    error_msg += "输入文本中存在中文\n"
                    break

        if self.process_type == 'n' or self.process_type == 'm':
            if self.is_t or self.is_h or self.is_r:
                error_msg += "-n -m 不能与 -h -t -r 混用"

        # TODO 其他有效性检查
        if len(error_msg) != 0:
            save_msg = wx.MessageDialog(self, error_msg, '错误')
            save_msg.ShowModal()
            save_msg.Destroy()
        else:
            print(cmd)
            self.command_exe(cmd)

    def download(self, evt):
        print("download")
        fd = wx.FileDialog(self, '把文件保存到何处', os.getcwd(), '.txt', 'TEXT file(*.txt)|*.txt', wx.FD_SAVE)
        if fd.ShowModal() == wx.ID_OK:
            file_name = fd.GetFilename()
            dir_name = fd.GetDirectory()
            try:
                with open(os.path.join(dir_name, file_name), 'w', encoding='utf-8') as f:
                    text = self.output_box.GetValue()
                    f.write(text)
                    save_msg = wx.MessageDialog(self, '文件已保存', '提示')
            except FileNotFoundError:
                save_msg = wx.MessageDialog(self, '保存失败,无效的保存路径', '提示')
        else:
            save_msg = wx.MessageDialog(self, '未选择保存路径', '错误')

        save_msg.ShowModal()
        save_msg.Destroy()

    def command_exe(self, command):
        try:
            dll = ctypes.windll.LoadLibrary("core.dll")
            call_func = dll.call_by_cmd
            call_func.argtypes = [ctypes.c_int, ctypes.POINTER(ctypes.c_char)]  # 参数类型为char指针
            call_func.restype = ctypes.c_char_p  # 返回类型为char指针
            result = dll.call_by_cmd(len(command), ctypes.c_char_p(command.encode('utf-8'))).decode("utf-8")
            self.output_box.AppendText("执行" + command + '\n')
            self.output_box.SetDefaultStyle(wx.TextAttr(wx.BLUE))  # 将后面输出字段设置为蓝色
            self.output_box.AppendText(result)
            self.output_box.SetDefaultStyle(wx.TextAttr(wx.NullColour))  # 将后面输出字段设置为默认颜色（黑色）
        except Exception as e:
            self.output_box.SetDefaultStyle(wx.TextAttr(wx.RED))  # 将后面输出字段设置为红色
            self.output_box.AppendText('ERROR！ ' + command + ' failed!' + '\n')
            self.output_box.AppendText(str(e))
            self.output_box.SetDefaultStyle(wx.TextAttr(wx.NullColour))  # 将后面输出字段设置为默认颜色（黑色）

        self.output_box.AppendText('执行结束\r\n')
        self.output_box.AppendText('-------\r\n')

    def select_input_type(self, evt):
        selected = self.select_input_type_box.GetSelection()
        if selected == 0:
            self.use_file = True
        else:
            self.use_file = False
        print(f"use file as input {self.use_file}")
        if self.use_file:
            self.file_picker.Show()
            self.text_input.Hide()
        else:
            self.text_input.Show()
            self.file_picker.Hide()

    def select_process_type(self, evt):
        selected = self.select_process_type_box.GetSelection()
        self.process_type = ['n', 'w', 'm', 'c'][selected]
        print(f"process type {self.process_type}")

    def select_file(self, evt):
        self.file_path = self.file_picker.GetPath()
        print(f"choose file from {self.file_path}")

    def on_h_check(self, evt):
        self.is_h = self.is_h_check.IsChecked()
        print(f"-h is {self.is_h}")
        if self.is_h:
            self.first_char_input.Show()
        else:
            self.first_char_input.Hide()

    def on_t_check(self, evt):
        self.is_t = self.is_t_check.IsChecked()
        print(f"-t is {self.is_t}")
        if self.is_t:
            self.last_char_input.Show()
        else:
            self.last_char_input.Hide()

    def on_r_check(self, evt):
        self.is_r = self.is_r_check.IsChecked()
        print(f"-r is {self.is_r}")


if __name__ == "__main__":
    app = wx.App()
    frame = MyFrame(parent=None, id=-1, title="命令行工具", pos=(50, 30), size=(1000, 600))  # 可通过修改size值来自行修改窗口大小
    frame.Show()

    """
    
extern "C" {
    __declspec(dllexport) char* __stdcall call_by_cmd(int len, char* cmd);
}

char* temp_ret_ptr = NULL;
char* call_by_cmd(int len, char* cmd) {
    temp_ret_ptr = NULL;

    istringstream input_cmd(cmd);
    vector<string> temp_vec(0);
    string out;

    while (input_cmd >> out) {
        temp_vec.push_back(out);
    }

    const int argc = temp_vec.size();
    char** argv = new char*[argc];
    for (int i = 0; i < argc;i++) {
        cout << temp_vec[i] << endl;
        argv[i] = new char[temp_vec[i].size() + 1];
        strcpy_s(argv[i], temp_vec[i].size() + 1,temp_vec[i].c_str());
    }

    if (read(argc, argv) < 0) {
        printf("File read failed\n");
        return 0;
    }

    if (handler == NULL) {
        printf("Unmatched params\n");
        return 0;
    }

    stringstream ss;
    streambuf* buffer = cout.rdbuf(); //oldbuffer,STDOUT的缓冲区
    cout.rdbuf(ss.rdbuf());

    ofstream out_stream("output.txt");
    (*handler)(&out_stream);

    string ret(ss.str());
    cout.rdbuf(buffer); // 重置，重新载入STDOUT的缓冲区

    delete[] temp_ret_ptr;
    temp_ret_ptr = new char[strlen(ret.c_str()) + 1];
    strcpy_s(temp_ret_ptr, strlen(ret.c_str()) + 1 ,ret.c_str());
    return temp_ret_ptr;
}
    """
    app.MainLoop()
