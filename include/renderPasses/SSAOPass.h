class SSAOPass : public RenderPass {
public:
    void init() override {
        shader_ = std::make_shared<Shader>("assets/shaders/ssao.vert", "assets/shaders/ssao.frag");
        framebuffer_ = std::make_unique<Framebuffer>(1920, 1080);
        framebuffer_->addDepthAttachment();
        framebuffer_->addColorAttachment();
    }

    void execute(RenderContext& context) override {
        framebuffer_->bind();
        shader_->bind();
        
        // Draw full-screen quad for SSAO calculation...
        
        shader_->unbind();
        framebuffer_->unbind();
    }

private:
    std::shared_ptr<Shader> shader_;
    std::unique_ptr<Framebuffer> framebuffer_;
};