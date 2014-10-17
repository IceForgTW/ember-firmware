# Base class for commands

module Smith
  module Client
    class Command

      include URLHelper
      include RequestHelper
      
      def initialize(printer, state, payload)
        @printer, @state, @payload = printer, state, payload
      end

      protected

      def acknowledge_command(message = '')
        request = post_request(acknowledge_endpoint,command: @payload.command, command_token: @payload.command_token, message: message)
        request.callback { Client.log_debug("Successfully acknowledged #{@payload.command.inspect} command (command token #{@payload.command_token.inspect})") }
      end

    end
  end
end
