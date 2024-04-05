void playVideo() {
    // Delay to achieve desired frame rate (e.g., 30 frames per second)
    const int delayMilliseconds = 1000 / 30;
    SDL_Delay(delayMilliseconds);
    // Open the video file
    AVFormatContext* formatContext = nullptr;
    if (avformat_open_input(&formatContext, filename, nullptr, nullptr) != 0) {
        std::cout << "Failed to open video file: " << filename << std::endl;
        
    }

    // Retrieve stream information
    if (avformat_find_stream_info(formatContext, nullptr) < 0) {
        std::cout << "Failed to retrieve stream information." << std::endl;
        
    }

    // Find the video stream index
    int videoStreamIndex = -1;
    for (unsigned int i = 0; i < formatContext->nb_streams; i++) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            break;
        }
    }

    if (videoStreamIndex == -1) {
        std::cout << "Failed to find a video stream." << std::endl;
        
    }

    // Get a pointer to the codec parameters for the video stream
    AVCodecParameters* codecParameters = formatContext->streams[videoStreamIndex]->codecpar;

    // Find the decoder for the video stream
    const AVCodec* codec = avcodec_find_decoder(codecParameters->codec_id);
if (codec == nullptr) {
    std::cout << "Failed to find a suitable codec." << std::endl;
}

    // Create a codec context for the decoder
    AVCodecContext* codecContext = avcodec_alloc_context3(codec);
    if (codecContext == nullptr) {
        std::cout << "Failed to allocate codec context." << std::endl;
        
    }

    // Initialize the codec context with the codec parameters
    if (avcodec_parameters_to_context(codecContext, codecParameters) < 0) {
        std::cout << "Failed to initialize codec context." << std::endl;
        
    }

    // Open the codec
    if (avcodec_open2(codecContext, codec, nullptr) < 0) {
        std::cout << "Failed to open codec." << std::endl;
        
    }
    // Create an SDL texture
    SDL_Texture* videoTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, codecContext->width, codecContext->height);
    if (!videoTexture) {
        std::cout << "Failed to create SDL texture: " << SDL_GetError() << std::endl;
        
    }

    // Allocate a frame for the decoded video frame
    AVFrame* frame = av_frame_alloc();

    // Create a packet for reading video frames
    AVPacket packet;

    // Read video frames
    while (av_read_frame(formatContext, &packet) >= 0) {
        // Check if the packet belongs to the video stream
        if (packet.stream_index == videoStreamIndex) {
            // Decode the video frame
            avcodec_send_packet(codecContext,&packet);
            int ret = avcodec_receive_frame(codecContext, frame);
            if (ret == 0) {
                // Convert the frame to a format compatible with SDL texture
                SDL_UpdateYUVTexture(videoTexture, nullptr, frame->data[0], frame->linesize[0], frame->data[1], frame->linesize[1], frame->data[2], frame->linesize[2]);

                SDL_RenderClear(renderer);
                // Copy the texture to the renderer
                SDL_RenderCopy(renderer, videoTexture, nullptr, nullptr);
                SDL_RenderPresent(renderer);
            }
        }

        // Free the packet
        av_packet_unref(&packet);

    }
    // Clean up resources
    av_frame_free(&frame);
    avcodec_close(codecContext);
    avformat_close_input(&formatContext);
}