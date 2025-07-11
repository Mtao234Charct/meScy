<h1>meScy - 轻量级C++ Web服务器框架</h1>
<p class="ds-markdown-paragraph">
   <i href="https://www.mtaoddn.site/images/ddnlogo.avif"></i>
                                             
</p>
<p class="ds-markdown-paragraph">meScy是一个轻量级、高性能的C++ Web服务器框架，专为快速构建Web应用和服务而设计。它结合了简洁的配置语法和高效的请求处理能力，让开发者能够轻松部署动态网页和API服务。</p>
<p class="ds-markdown-paragraph">
<a href="https://www.mtaoddn.site/" target="_blank" rel="noreferrer">🌐 访问官网</a>
</p>
                                                        <h2>主要特性</h2>
                                                        <ul>
                                                            <li>
                                                                <p class="ds-markdown-paragraph">
                                                                    🚀 <strong>极简配置</strong>
                                                                    ：使用专有的<code>.esr</code>
                                                                    配置文件语法，快速定义路由和服务器行为
                                                                </p>
                                                            </li>
                                                            <li>
                                                                <p class="ds-markdown-paragraph">
                                                                    ⚡ <strong>高性能处理</strong>
                                                                    ：基于C++17和现代异步I/O模型构建
                                                                </p>
                                                            </li>
                                                            <li>
                                                                <p class="ds-markdown-paragraph">
                                                                    📁 <strong>静态文件服务</strong>
                                                                    ：自动处理HTML/CSS/JS等静态资源
                                                                </p>
                                                            </li>
                                                            <li>
                                                                <p class="ds-markdown-paragraph">
                                                                    🔌 <strong>可扩展架构</strong>
                                                                    ：支持中间件和自定义处理逻辑
                                                                </p>
                                                            </li>
                                                            <li>
                                                                <p class="ds-markdown-paragraph">
                                                                    🔒 <strong>安全可靠</strong>
                                                                    ：内置基础防护措施，防止常见Web攻击
                                                                </p>
                                                            </li>
                                                            <li>
                                                                <p class="ds-markdown-paragraph">
                                                                    📦 <strong>轻量级</strong>
                                                                    ：核心实现小于20MB，无额外依赖
                                                                </p>
                                                            </li>
                                                        </ul>
 <h2>快速开始</h2>
 <h3>安装依赖</h3>
 <div class="md-code-block md-code-block-light">
                                                      
<pre>
<span class="token comment"># 必需组件</span>
MinGW64 15.1.0
</pre>
 或：直接下载二进制版本
</div>
<h3>编译meScy（如果使用二进制版本请跳过此处</h3>
<div class="md-code-block md-code-block-light">
g++ meScy.cpp -o meScy.exe -lssl -lcrypto -lws2_32 -L"Your openssl lib" -I"Your openssl inc"
 </div>
<h3>
 基础配置 (<code>config.esr</code>)
</h3>
<div class="md-code-block md-code-block-light">
                                                            
<pre>
bindon localhost
listenon 8080

add froute {
    /about     about.html
    /api/data api_handler.cpp
}
</pre>
</div>
<h3>启动服务器</h3>
<div class="md-code-block md-code-block-light">
<pre>./meScy</pre>
</div>
<h2>配置文件语法</h2>
<p class="ds-markdown-paragraph">
    meScy使用<code>.esr</code>
    (Easy Script Route)文件配置服务器：
</p>
<div class="md-code-block md-code-block-light">
                                                            
<pre># 绑定地址 (默认: localhost)
bindon localhost

# 监听端口 (默认: 8080)
listenon 8080

# 添加文件路由
add froute {
    /home      home.html
    /contact   contact.html
    /blog      posts/blog.html
}

#设置默认页面
set default index.htm
#注：以上配置meScy与meScy-http-only通用
   
#配置SSL (仅限meScy)
cert server.crt
privateKey server.key
   
</pre>
</div>
<h2>示例项目结构</h2>
<div class="md-code-block md-code-block-light">
                                                            
<pre>project/
├── config.esr        # 服务器配置
├── public/           # 静态资源
│   ├── index.html
│   ├── about.html
│   └── styles.css
│
└── build/
    └── meScy         # 服务器可执行文件
# 注意：采用此布局时请使用绝对路径配置路由
</pre>
</div>
<h2>性能基准</h2>
<div class="markdown-table-wrapper">
<table>
                                                                <thead>
                                                                    <tr>
                                                                        <th>测试条件</th>
                                                                        <th>请求/秒</th>
                                                                        <th>内存占用</th>
                                                                        <th>延迟(avg)</th>
                                                                    </tr>
                                                                </thead>
                                                                <tbody>
                                                                    <tr>
                                                                        <td>100并发</td>
                                                                        <td>12,500</td>
                                                                        <td>15MB</td>
                                                                        <td>8ms</td>
                                                                    </tr>
                                                                    <tr>
                                                                        <td>500并发</td>
                                                                        <td>8,200</td>
                                                                        <td>22MB</td>
                                                                        <td>60ms</td>
                                                                    </tr>
                                                                    <tr>
                                                                        <td>1000并发</td>
                                                                        <td>5,600</td>
                                                                        <td>35MB</td>
                                                                        <td>180ms</td>
                                                                    </tr>
                                                                </tbody>
                                                            </table>
                                                        </div>
                                                        <p class="ds-markdown-paragraph">*测试环境: Windows Server 2025, Intel i7-12700, 32GB RAM*</p>
                                                        <h2>应用场景</h2>
                                                        <ol start="1">
                                                            <li>
                                                                <p class="ds-markdown-paragraph">
                                                                    <strong>静态网站托管</strong>
                                                                    - 替代Nginx/Apache的轻量级选择
                                                                </p>
                                                            </li>
                                                            <li>
                                                                <p class="ds-markdown-paragraph">
                                                                    <strong>动态路由支持</strong>
                                                                    - 快速构建用户友好url
                                                                </p>
                                                            </li>
                                                            <li>
                                                                <p class="ds-markdown-paragraph">
                                                                    <strong>物联网网关</strong>
                                                                    - 低资源消耗的设备控制接口
                                                                </p>
                                                            </li>
                                                            <li>
                                                                <p class="ds-markdown-paragraph">
                                                                    <strong>教育项目</strong>
                                                                    - 学习网络编程和C++的实践平台
                                                                </p>
                                                            </li>
                                                            <li>
                                                                <p class="ds-markdown-paragraph">
                                                                    <strong>原型开发</strong>
                                                                    - 快速验证Web应用概念
                                                                </p>
                                                            </li>
                                                        </ol>
                                                        <h2>贡献指南</h2>
                                                        <p class="ds-markdown-paragraph">欢迎贡献代码！请遵循以下流程：</p>
                                                        <ol start="1">
                                                            <li>
                                                                <p class="ds-markdown-paragraph">在 https://www.mtaoddn.site 上注册账户</p>
                                                            </li>
                                                            <li>
                                                                <p class="ds-markdown-paragraph">
                                                                    发送贡献帖子 (<code>https://www.mtaoddn.site/index.php?where=doct</code>
                                                                    )
                                                                </p>
                                                            </li>
                                                            <li>
                                                                <p class="ds-markdown-paragraph">
                                                                    发送邮件给开发者 (<code>admin@mtaoddn.site</code>)
                                                                </p>
                                                            </li>
                                                            <li>
                                                                <p class="ds-markdown-paragraph">
                                                                    等待开发者回信 
                                                                </p>
                                                            </li>
                                                            <li>
                                                                <p class="ds-markdown-paragraph">按照开发者给予的链接提交代码</p>
                                                            </li>
                                                        </ol>
                                                        <h2>路线图</h2>
                                                        <ul>
                                                            <li>
                                                                <p class="ds-markdown-paragraph">v0.2: 添加ESR支持</p>
                                                            </li>
                                                            <li>
                                                                <p class="ds-markdown-paragraph">v0.3: 添加https支持</p>
                                                            </li>
                                                            <li>
                                                                <p class="ds-markdown-paragraph">v0.4: 支持WebSocket协议</p>
                                                            </li>
                                                            <li>
                                                                <p class="ds-markdown-paragraph">v0.5: 动态路由支持</p>
                                                            </li>
                                                            <li>
                                                               <p class="ds-markdown-paragraph">v0.6：分离meScy与meScy-http-only，实现双支持</p>
                                                            </li>
                                                        </ul>
                                                        
<hr>
<p class="ds-markdown-paragraph">
<a href="https://github.com/Mtao234Charct/meScy" target="_blank" rel="noreferrer">
   💻 GitHub仓库
</a>
 |
   <br>
<a href="https://www.mtaoddn.site/index.php?where=doct" target="_blank" rel="noreferrer">
   📚 完整文档
</a>
 |
   <br>
<a href="https://github.com/Mtao234Charct/meScy/issues" target="_blank" rel="noreferrer">
   🐞 报告问题
</a>
 |
   <br>
                                                            
</p>
<p class="ds-markdown-paragraph">
<strong>
   用C++的力量，构建Web的未来
</strong>
</p>
