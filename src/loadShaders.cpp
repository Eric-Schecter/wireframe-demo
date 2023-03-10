//////////////////////////////////////////////////////////////////////////////
//
//  --- LoadShaders.cxx ---
//
//////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <iostream>

#include <GL/glew.h>
#include "loadShaders.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//----------------------------------------------------------------------------

static const GLchar*
ReadShader( const char* filename )
{
    FILE* infile;
    fopen_s(&infile, filename, "rb");

    if ( !infile ) {
#ifdef _DEBUG
        std::cerr << "Unable to open file '" << filename << "'" << std::endl;
#endif /* DEBUG */
        return NULL;
    }

    fseek( infile, 0, SEEK_END );
    int len = ftell( infile );
    fseek( infile, 0, SEEK_SET );

    GLchar* source = new GLchar[len+1];

    fread( source, 1, len, infile );
    fclose( infile );

    source[len] = 0;

    return const_cast<const GLchar*>(source);
}

//----------------------------------------------------------------------------

GLuint
LoadShaders(ShaderInfo* shaders, const GLchar** feedbackVaryings)
{

    if ( shaders == NULL ) { return 0; }

    GLuint program = glCreateProgram();

    ShaderInfo* entry = shaders;

    while ( entry->type != GL_NONE ) {
        GLuint shader = glCreateShader( entry->type );

        entry->shader = shader;

        const GLchar* source = ReadShader( entry->filename );

        if ( source == NULL ) {
            for ( entry = shaders; entry->type != GL_NONE; ++entry ) {
                glDeleteShader( entry->shader );
                entry->shader = 0;
            }

            return 0;
        }

        glShaderSource( shader, 1, &source, NULL );
        delete [] source;

        glCompileShader( shader );

        GLint compiled;
        glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );

        if ( !compiled ) {
#ifdef _DEBUG
            GLsizei len;
            glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &len );

            GLchar* log = new GLchar[len+1];
            glGetShaderInfoLog( shader, len, &len, log );
            std::cerr << "Shader compilation failed: " << log << std::endl;
            delete [] log;
#endif /* DEBUG */

            return 0;
        }

        glAttachShader( program, shader );

        ++entry;
    }

    if (feedbackVaryings!=nullptr) {
        glTransformFeedbackVaryings(program, 1, feedbackVaryings, GL_INTERLEAVED_ATTRIBS);
    }

    glLinkProgram( program );

    GLint linked;
    glGetProgramiv( program, GL_LINK_STATUS, &linked );

    if ( !linked ) {
#ifdef _DEBUG
        GLsizei len;
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &len );

        GLchar* log = new GLchar[len+1];
        glGetProgramInfoLog( program, len, &len, log );
        std::cerr << "Shader linking failed: " << log << std::endl;
        delete [] log;
#endif /* DEBUG */

        for ( entry = shaders; entry->type != GL_NONE; ++entry ) {
            glDeleteShader( entry->shader );
            entry->shader = 0;
        }

        return 0;
    }

    return program;
}

GLuint
LoadShadersPipeline(ShaderInfoPipeline* shaders, const GLchar** feedbackVaryings)
{
    if (shaders == NULL) { return 0; }
    GLuint pipeline = 0;
    glCreateProgramPipelines(1, &pipeline);

    ShaderInfoPipeline* entry = shaders;

    while (entry->type != GL_NONE) {
        const GLchar* source = ReadShader(entry->filename);

        // generate, init, compile, link shader program, set program parameter to seperated
        GLuint program = glCreateShaderProgramv(entry->type, 1, &source);
        *(entry->program) = program;

        GLint linked;
        glGetProgramiv(program,GL_LINK_STATUS, &linked);
        if (!linked) {
            char message[1024];
            glGetProgramInfoLog(program, sizeof(message), nullptr, message);
            std::cout << message << "\n";
        }

        glUseProgramStages(pipeline, entry->bit, *(entry->program));

        ++entry;
    }

    return pipeline;
}

//----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cplusplus


