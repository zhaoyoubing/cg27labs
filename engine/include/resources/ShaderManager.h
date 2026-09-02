#pragma once

#include "device/GPUPipeline.h"

#include <unordered_map>
#include <memory>

// need to extend it to compute shader in the future
class ShaderManager
{
public:
    std::shared_ptr<GPUPipeline> load(const std::string& name, const std::string& vertexFile,
        const std::string& fragmentFile) 
    {
        Shader vertShader(ShaderStage::Vertex, vertexFile);
        Shader fragShader(ShaderStage::Fragment, fragmentFile);

        // Group the compiled stages into a Pipeline (Vertex-Fragment pair)
        std::vector<Shader*> shaderStages = { &vertShader, &fragShader };
        std::shared_ptr<GPUPipeline> pipeline = std::make_shared<GPUPipeline>(shaderStages);
        pipelines_[name] = pipeline;

        return pipeline;
    }

    std::shared_ptr<GPUPipeline> get(const std::string& name) const {
            return pipelines_.at(name);
    }

    void clear() {
        pipelines_.clear();
    }

private:
    std::unordered_map<
        std::string,
        std::shared_ptr<GPUPipeline>
    > pipelines_;
};